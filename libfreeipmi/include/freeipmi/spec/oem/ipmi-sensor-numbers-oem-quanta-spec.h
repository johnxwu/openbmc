/*
 * Copyright (C) 2003-2015 FreeIPMI Core Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef IPMI_SENSOR_NUMBERS_OEM_QUANTA_SPEC_H
#define IPMI_SENSOR_NUMBERS_OEM_QUANTA_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Quanta S99Q/Dell FS12-TY
 */
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_MEMORY              0x60
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_PCI_SENSORID        0x81
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_QPI_SENSORID        0x82
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_INT_SENSORID        0x83
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_SOFTWARE_NMI        0xA5

#ifdef __cplusplus
}
#endif

#endif /* IPMI_SENSOR_NUMBERS_OEM_QUANTA_SPEC_H */
