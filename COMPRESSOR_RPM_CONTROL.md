# Compressor RPM Control Implementation

## Overview
This implementation adds pressure-based RPM control to the compressor system to maintain 13-14 bar refrigerant pressure and reduce cycling.

## Changes Made

### 1. VCU (Stm32-vcu) Changes

#### A. New Parameters (include/param_prj.h)
- **compTargetRPM** (ID: 166)
  - Category: CAT_AIRCON
  - Unit: RPM
  - Range: 3000-6000 RPM
  - Default: 4200 RPM
  - Description: Manual target RPM setpoint (used when no CAN value received)

- **compRPMHyst** (ID: 167)
  - Category: CAT_AIRCON
  - Unit: RPM
  - Range: 50-500 RPM
  - Default: 200 RPM
  - Description: Hysteresis band to prevent cycling

#### B. New Value Entry
- **compressRPMTarget** (ID: 2134)
  - Unit: RPM
  - Description: Received target RPM from BMW controller (via CAN mapping)

#### C. Updated Control Logic (src/OutlanderCompressor.cpp)
The compressor now uses hysteresis-based control:
- **RPM < (target - hysteresis)**: Increase compressor speed (0x35)
- **RPM > (target + hysteresis)**: Decrease compressor speed (0x20)
- **Within hysteresis band**: Maintain moderate speed (0x25)

Priority: Uses `compressRPMTarget` (from CAN) if available, otherwise falls back to `compTargetRPM` (manual parameter).

### 2. BMW HIKA KCAN Controller Changes

#### A. New Constants (src/main.cpp, lines 84-92)
```cpp
// Pressure control
static constexpr float COMP_TARGET_PRESSURE  = 13.5f;  // Target 13-14 bar midpoint
static constexpr float COMP_RPM_MIN          = 3200;   // Minimum compressor RPM
static constexpr float COMP_RPM_MAX          = 5000;   // Maximum compressor RPM
static constexpr float COMP_RPM_DEFAULT      = 4200;   // Default when no sensor data
static constexpr float COMP_PRESSURE_LOW     = 12.0f;  // Below this, increase RPM
static constexpr float COMP_PRESSURE_HIGH    = 15.0f;  // Above this, decrease RPM

// Evaporator temperature control
static constexpr float COMP_TARGET_EVAP_TEMP = 4.0f;   // Target evaporator temp (°C)
static constexpr float COMP_EVAP_TEMP_LOW    = 2.0f;   // Below this, decrease RPM (too cold)
static constexpr float COMP_EVAP_TEMP_HIGH   = 6.0f;   // Above this, increase RPM (too warm)
```

#### B. New Function: compressorRpmForPressureAndTemp()
Calculates target RPM based on current refrigerant pressure AND evaporator temperature:

**Pressure Control:**
- **Pressure < 12 bar**: Increases RPM by 300 RPM per bar deficit
- **Pressure > 15 bar**: Decreases RPM by 200 RPM per bar excess
- **12-15 bar range**: No pressure adjustment

**Evaporator Temperature Control:**
- **Temp < 2°C**: Decreases RPM by 150 RPM per °C (prevents icing)
- **Temp > 6°C**: Increases RPM by 100 RPM per °C (increases cooling)
- **2-6°C range**: No temperature adjustment
- **Target: ~4°C** (optimal cooling without freezing)

Both adjustments are combined and applied to the base 4200 RPM, then clamped to 3200-5000 RPM range.

#### C. Extended CAN Message (0x500)
Message length extended from 5 to 7 bytes:
- Byte [0]: Heater enable (0/1)
- Byte [1]: Compressor enable (0/1)
- Byte [2]: Fan PWM (0-100%)
- Byte [3]: Pressure (bar × 10)
- Byte [4]: Evaporator temp (signed int8)
- **Byte [5]: Target RPM high byte (RPM ÷ 256)** ← NEW
- **Byte [6]: Target RPM low byte (RPM % 256)** ← NEW

## Configuration Instructions

### VCU Web Interface Setup

1. Navigate to **CAN Mapping** → **Add RX Mapping**

2. Add the following new mapping:

   **Compressor Target RPM:**
   - CAN ID: `0x500` (hex) / `1280` (decimal)
   - Start Bit: `40` (byte 5, bit 0)
   - Length: `16` bits
   - Signed: `No`
   - Target Parameter: `compressRPMTarget`
   - Gain: `1`
   - Offset: `0`

3. Save and restart the VCU

### Tuning Parameters

#### VCU Parameters (Web Interface):
- **compTargetRPM**: Fallback target when no CAN data (default: 4200 RPM)
- **compRPMHyst**: Control deadband (default: 200 RPM)
  - Larger values = less cycling but looser pressure control
  - Smaller values = tighter control but more responsive switching

#### BMW Controller Constants (requires recompiling):
Edit `/Users/jamiejones/Documents/PlatformIO/BMW_HIKA_KCAN/src/main.cpp`:

**Pressure Control:**
- **COMP_TARGET_PRESSURE**: Ideal pressure midpoint (default: 13.5 bar)
- **COMP_PRESSURE_LOW**: Below this, increase RPM (default: 12.0 bar)
- **COMP_PRESSURE_HIGH**: Above this, decrease RPM (default: 15.0 bar)

**Temperature Control:**
- **COMP_TARGET_EVAP_TEMP**: Ideal evaporator temp (default: 4.0°C)
- **COMP_EVAP_TEMP_LOW**: Below this, decrease RPM (default: 2.0°C)
- **COMP_EVAP_TEMP_HIGH**: Above this, increase RPM (default: 6.0°C)

**RPM Limits:**
- **COMP_RPM_MIN**: Minimum compressor RPM (default: 3200)
- **COMP_RPM_MAX**: Maximum compressor RPM (default: 5000)
- **COMP_RPM_DEFAULT**: Nominal RPM in target zones (default: 4200)

## How It Works

### Dual-Input Control Loop

1. **BMW Controller** reads:
   - Refrigerant pressure from K-CAN (0x2D2)
   - Evaporator temperature from IHKA UDS (0x678)

2. **Controller** calculates target RPM using `compressorRpmForPressureAndTemp()`:

   **Pressure-based adjustment:**
   - Low pressure (< 12 bar) → +300 RPM per bar deficit
   - High pressure (> 15 bar) → -200 RPM per bar excess
   - Target range (12-15 bar) → No pressure adjustment

   **Temperature-based adjustment:**
   - Too cold (< 2°C) → -150 RPM per °C (prevents icing)
   - Too warm (> 6°C) → +100 RPM per °C (increases cooling)
   - Target range (2-6°C) → No temperature adjustment

   Both adjustments combine to give final target RPM (3200-5000 range)

3. **Controller** sends target RPM to VCU via CAN 0x500 bytes [5:6]
4. **VCU** receives target RPM and stores in `compressRPMTarget`
5. **VCU** compares actual compressor RPM (from 0x388) to target
6. **VCU** adjusts compressor speed command (0x185) using hysteresis control

### Example Scenarios

#### Scenario 1: Normal Operation
- Pressure: 13.2 bar (in range 12-15) → Pressure adjustment: 0 RPM
- Evap Temp: 4.5°C (in range 2-6) → Temp adjustment: 0 RPM
- Calculated Target RPM: 4200 + 0 + 0 = **4200 RPM**
- Actual RPM: 4250 RPM
- Error: +50 RPM (within ±200 hysteresis)
- **VCU Action**: Send 0x25 (maintain speed)

#### Scenario 2: Low Pressure
- Pressure: 11.5 bar (0.5 below 12.0) → Pressure adjustment: +150 RPM
- Evap Temp: 4.0°C (in range) → Temp adjustment: 0 RPM
- Calculated Target RPM: 4200 + 150 = **4350 RPM**
- Actual RPM: 3800 RPM
- Error: -550 RPM (below -200 hysteresis)
- **VCU Action**: Send 0x35 (increase speed)

#### Scenario 3: Evaporator Too Warm
- Pressure: 13.0 bar (in range) → Pressure adjustment: 0 RPM
- Evap Temp: 8°C (2°C above 6) → Temp adjustment: +200 RPM
- Calculated Target RPM: 4200 + 200 = **4400 RPM**
- Actual RPM: 4250 RPM
- Error: -150 RPM (within ±200 hysteresis)
- **VCU Action**: Send 0x25 (maintain - within deadband)

#### Scenario 4: Evaporator Too Cold (Risk of Icing)
- Pressure: 14.0 bar (in range) → Pressure adjustment: 0 RPM
- Evap Temp: 1°C (1°C below 2) → Temp adjustment: -150 RPM
- Calculated Target RPM: 4200 - 150 = **4050 RPM**
- Actual RPM: 4250 RPM
- Error: +200 RPM (at hysteresis boundary)
- **VCU Action**: Send 0x20 (decrease speed to prevent icing)

#### Scenario 5: Combined High Pressure + Too Cold
- Pressure: 16.0 bar (1 bar above 15) → Pressure adjustment: -200 RPM
- Evap Temp: 1°C (1°C below 2) → Temp adjustment: -150 RPM
- Calculated Target RPM: 4200 - 200 - 150 = **3850 RPM**
- **Result:** Both inputs agree to reduce compressor speed

**Overall Result:** System maintains 13-14 bar pressure AND ~4°C evaporator temperature with minimal cycling.

## Benefits

1. **Reduced Cycling**: Hysteresis prevents rapid on/off switching
2. **Pressure Regulation**: Automatically adjusts RPM to maintain 13-14 bar target
3. **Temperature Control**: Maintains optimal 4°C evaporator temperature
4. **Prevents Icing**: Reduces RPM if evaporator gets too cold (< 2°C)
5. **Improves Cooling**: Increases RPM if evaporator too warm (> 6°C)
6. **Adaptive Control**: Responds to load changes (fan speed, ambient temp, cabin heat load)
7. **Dual-Input Strategy**: Both pressure and temperature work together for optimal performance
8. **Manual Override**: Can still set RPM manually via `compTargetRPM` parameter
9. **Monitoring**: `compressRPMTarget` shows what the controller is requesting

## Troubleshooting

### Compressor not responding to pressure changes
- Check CAN mapping for `compressRPMTarget` is configured correctly
- Verify BMW controller is sending 7-byte messages on 0x500
- Check `compressRPMTarget` value in VCU web interface (should update every 100ms)

### Excessive cycling
- Increase `compRPMHyst` (try 300-400 RPM)
- Verify pressure sensor reading (0x2D2 on K-CAN)
- Check fan curve is working properly

### RPM target seems wrong
- Verify BMW controller pressure reading matches actual
- Adjust `COMP_PRESSURE_LOW` and `COMP_PRESSURE_HIGH` constants
- Check RPM calculation gains (300 RPM/bar for deficit, 200 RPM/bar for excess)

## Testing Checklist

- [ ] Verify CAN mapping receives `compressRPMTarget` (should be 3200-5000)
- [ ] Confirm compressor RPM follows target with hysteresis
- [ ] Check pressure stabilizes at 13-14 bar during operation
- [ ] Monitor for cycling behavior (should be minimal)
- [ ] Test across different ambient temperatures
- [ ] Verify fallback to manual `compTargetRPM` if CAN fails

## Files Modified

### VCU (Stm32-vcu):
- `include/param_prj.h`: Added parameters and value entry
- `src/OutlanderCompressor.cpp`: Updated control logic

### BMW Controller (BMW_HIKA_KCAN):
- `src/main.cpp`: Added RPM calculation and extended CAN message
