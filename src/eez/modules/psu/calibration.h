/*
 * EEZ Modular Firmware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace eez {
namespace psu {
/// Channel calibration procedure.
namespace calibration {

////////////////////////////////////////////////////////////////////////////////////

class CalibrationBase {
    friend struct Value;

public:
    void getCalibrationChannel(int &slotIndex, int &subchannelIndex);
    bool hasSupportForCurrentDualRange();
    CalibrationValueType getCalibrationValueType();
    bool isCalibrationExists();
    void getMaxValue(CalibrationValueType valueType, float &value, Unit &unit);
    float roundCalibrationValue(Unit unit, float value);
    bool isCalibrationValueTypeSelectable();
    float getDacValue(CalibrationValueType valueType);

protected:
    int m_slotIndex;
    int m_subchannelIndex;
};

////////////////////////////////////////////////////////////////////////////////

class CalibrationEditor;

/// Calibration parameters for the voltage or current during calibration procedure.
struct Value {
    CalibrationBase &editor;

    CalibrationValueType type;
    int currentPointIndex;
    bool isPointSet[MAX_CALIBRATION_POINTS];
    CalibrationValueConfiguration configuration;

    Value(CalibrationEditor &editor_, CalibrationValueType type_);

    void reset();

    void setCurrentPointIndex(int currentPointIndex);

    void setDacValue(float value);
    float getDacValue();

    float readAdcValue();

    bool checkValueAndAdc(float value, float adc);
    void setValueAndAdc(float value, float adc);

    bool checkPoints();
};

////////////////////////////////////////////////////////////////////////////////

class CalibrationEditor : public CalibrationBase {
public:
    bool isEnabled() { return m_enabled; }

    /// Start calibration procedure on the channel.
    void start(int slotIndex, int subchannelIndex);

    /// Stop calibration procedure.
    void stop();

    ChannelMode getChannelMode();

    float getAdcValue(CalibrationValueType valueType);

    void selectCurrentRange(int8_t range);

    Value &getVoltage();
    Value &getCurrent();

    void setVoltage(float value);
    void setCurrent(float value);

    void copyValuesFromChannel();

    /// Is calibration remark is set.
    bool isRemarkSet();

    /// Get currently set remark.
    const char *getRemark();

    /// Set calibration remark.
    void setRemark(const char *value, size_t len);

    /// Are all calibration parameters entered?
    bool canSave(int16_t &scpiErr, int16_t *uiErr = nullptr);

    /// Save calibration parameters entered during calibration procedure.
    bool save();

private:
    bool m_enabled;

    Value m_voltageValue = Value(*this, CALIBRATION_VALUE_U);

    int8_t m_currentRangeSelected = 0;
    Value m_currentsValue[2] = {
        Value(*this, CALIBRATION_VALUE_I_HI_RANGE),
        Value(*this, CALIBRATION_VALUE_I_LOW_RANGE)
    };

    bool m_remarkSet;
    char m_remark[CALIBRATION_REMARK_MAX_LENGTH + 1];

    void doStart();
};

extern CalibrationEditor g_editor;

////////////////////////////////////////////////////////////////////////////////

class CalibrationViewer : public CalibrationBase {
public:
    void start(int slotIndex, int subchannelIndex);
};

extern CalibrationViewer g_viewer;

////////////////////////////////////////////////////////////////////////////////

/// Clear calibration parameters for given channel.
bool clear(int slotIndex, int subchannelIndex, int *err);

void clearCalibrationConf(CalibrationConfiguration *calConf);

float remapValue(float value, CalibrationValueConfiguration &cal);

bool onHighPriorityThreadMessage(uint8_t type, uint32_t param);

} // namespace calibration
} // namespace psu
} // namespace eez
