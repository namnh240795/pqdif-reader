# PQDIF JSON Export Field Reference

This document describes every field in the JSON output produced by `PqFile.getJSON()`.

## Top-Level Structure

```json
{
  "fileName": "string",
  "records": [ ... ]
}
```

| Field | Type | Description |
|-------|------|-------------|
| `fileName` | `string` | Path to the source PQDIF file |
| `records` | `RecordHolder[]` | Array of all records in the file |

### RecordHolder

| Field | Type | Description |
|-------|------|-------------|
| `recIndex` | `number` | Zero-based position of the record in the file |
| `recType` | `number` | Record type: `1`=Container, `2`=DataSource, `3`=MonitorSettings, `4`=Observation |
| `record` | `Object` | The parsed record data (structure depends on `recType`) |

---

## Container Record (`recType: 1`)

The first record in every PQDIF file. Contains file metadata and compression settings.

| Field | Type | Description |
|-------|------|-------------|
| `recordType` | `string` | Always `"Container"` |
| `versionInfo` | `number[]` | PQDIF format version as `[major, minor, update, revision]` (e.g. `[1,5,1,5]`) |
| `fileName` | `string` | Original file name when created |
| `creation` | `string\|null` | ISO 8601 timestamp of file creation |
| `lastSaved` | `string\|null` | ISO 8601 timestamp of last save |
| `timesSaved` | `number\|null` | Number of times the file has been saved |
| `language` | `string` | Language of the file (e.g. `"US English"`) |
| `title` | `string` | Arbitrary title |
| `subject` | `string` | Subject or description |
| `author` | `string` | File author |
| `keywords` | `string` | Search keywords |
| `comments` | `string` | Free-form comments |
| `lastSavedBy` | `string` | Name of last person/application to save |
| `application` | `string` | Application that created the file |
| `security` | `string` | Security setting |
| `owner` | `string` | File owner |
| `copyright` | `string` | Copyright notice |
| `trademarks` | `string` | Trademark notices |
| `notes` | `string` | Free-form notes |
| `compressionStyleID` | `number` | `0`=None, `1`=Total-file (deprecated), `2`=Record-level |
| `compressionAlgorithmID` | `number` | `0`=None, `1`=ZLIB, `64`=PKZIP (deprecated) |
| `contact` | `ContactInfo` | Contact information object |

### ContactInfo

| Field | Type | Description |
|-------|------|-------------|
| `name` | `string` | Contact name |
| `address1` | `string` | Address line 1 |
| `address2` | `string` | Address line 2 |
| `city` | `string` | City |
| `state` | `string` | State or province |
| `postalCode` | `string` | Postal/ZIP code |
| `country` | `string` | Country |
| `phoneVoice` | `string` | Voice phone number |
| `phoneFAX` | `string` | FAX phone number |
| `email` | `string` | Email address |

---

## DataSource Record (`recType: 2`)

Describes the instrument or system that recorded the data.

| Field | Type | Description |
|-------|------|-------------|
| `recordType` | `string` | Always `"DataSource"` |
| `dataSourceTypeID` | `string\|null` | GUID hex: data source type (e.g. `e6b51730...` = Measured) |
| `vendorID` | `string\|null` | GUID hex: instrument vendor |
| `equipmentID` | `string\|null` | GUID hex: instrument equipment type |
| `serialNumber` | `string` | Instrument serial number |
| `version` | `string` | Instrument or software version |
| `name` | `string` | Data source name (e.g. `"PQDIF Convert"`) |
| `owner` | `string` | Data source owner |
| `location` | `string` | Measurement location description |
| `timeZone` | `string` | Time zone identifier |
| `latitude` | `number` | Geographic latitude |
| `longitude` | `number` | Geographic longitude |
| `comments` | `string` | Free-form comments |
| `utcToLST` | `number\|null` | UTC to Local Standard Time offset in seconds |
| `effective` | `string\|null` | ISO 8601 timestamp when this definition became effective |
| `channelDefinitions` | `ChannelDefinition[]` | Array of channel definitions |
| `customSourceInfo` | `CustomSourceInfo\|null` | Instrument metadata |

### ChannelDefinition

| Field | Type | Description |
|-------|------|-------------|
| `channelName` | `string` | Human-readable channel name (e.g. `"Waveform VAN"`, `"SS RMS IA"`) |
| `phaseID` | `number` | Phase identifier (see Phase IDs below) |
| `otherChannelIdentifier` | `string` | Alternative channel identifier |
| `groupName` | `string` | Channel group name |
| `quantityTypeID` | `string\|null` | GUID hex: type of quantity (see Quantity Type IDs below) |
| `quantityMeasuredID` | `number` | What is being measured (see Quantity Measured IDs below) |
| `physicalChannel` | `number` | Physical channel number on the instrument |
| `quantityName` | `string` | Name of the measured quantity |
| `primarySeriesIdx` | `number` | Index of the primary series definition |
| `seriesDefinitions` | `SeriesDefinition[]` | Array of series definitions for this channel |

### SeriesDefinition

| Field | Type | Description |
|-------|------|-------------|
| `valueTypeID` | `string\|null` | GUID hex: type of value (see Value Type IDs below) |
| `quantityUnitsID` | `number` | Units of the quantity (see Quantity Units below) |
| `quantityCharacteristicID` | `string\|null` | GUID hex: characteristic of the quantity (see Quantity Characteristic IDs below) |
| `quantitySignificantDigitsID` | `number\|null` | Number of significant digits |
| `quantityResolutionID` | `number\|null` | Resolution of the quantity |
| `storageMethodID` | `number` | Storage method: `1`=Values, `2`=Scaled, `4`=Increment |
| `valueTypeName` | `string` | Optional descriptive name for the value type |
| `hintGreekPrefixID` | `number\|null` | Greek prefix hint (kilo, mega, etc.) |
| `hintPreferredUnitsID` | `number\|null` | Preferred units display hint |
| `hintDefaultDisplayID` | `number\|null` | Default display format hint |
| `probInterval` | `number\|null` | Probability interval |
| `probPercentile` | `number\|null` | Probability percentile |
| `seriesNominalQuantity` | `number\|null` | Nominal quantity value (e.g. 34000 for 34kV) |

### CustomSourceInfo

| Field | Type | Description |
|-------|------|-------------|
| `instrumentTypeID` | `string\|null` | GUID hex: instrument type |
| `instrumentModelName` | `string` | Instrument model name |
| `instrumentModelNumber` | `string` | Instrument model number |
| `contact` | `ContactInfo` | Contact information (same structure as Container contact) |

---

## MonitorSettings Record (`recType: 3`)

Configuration parameters for the monitoring instrument.

| Field | Type | Description |
|-------|------|-------------|
| `recordType` | `string` | Always `"MonitorSettings"` |
| `effective` | `string\|null` | ISO 8601 timestamp when settings became effective |
| `timeInstalled` | `string\|null` | ISO 8601 timestamp when instrument was installed |
| `timeRemoved` | `string\|null` | ISO 8601 timestamp when instrument was removed |
| `useCalibration` | `boolean` | Whether calibration is applied |
| `useTransducer` | `boolean` | Whether transducer ratios are applied |
| `nominalFrequency` | `number\|null` | Nominal system frequency in Hz (e.g. `60`) |
| `physicalConnection` | `number\|null` | Physical connection type (see Physical Connection IDs below) |
| `comments` | `string` | Free-form comments |
| `lsttoLocal` | `number\|null` | Local Standard Time to Local Time offset in seconds |
| `channelSettings` | `ChannelSetting[]` | Array of per-channel settings |

### ChannelSetting

| Field | Type | Description |
|-------|------|-------------|
| `channelDefnIdx` | `number` | Index into the data source's channel definitions |
| `triggerTypeID` | `number\|null` | Trigger type (see Trigger Type IDs below) |
| `fullScale` | `number\|null` | Full-scale range of the ADC |
| `noiseFloor` | `number\|null` | Noise floor level |
| `triggerShapeParam` | `number[]\|null` | Trigger shape parameters (3 values) |
| `transformer` | `TransformerInfo` | Transducer/transformer settings |
| `calibration` | `CalibrationInfo` | Calibration settings |
| `triggerThresholds` | `TriggerThresholds` | Trigger threshold values |

### TransformerInfo

| Field | Type | Description |
|-------|------|-------------|
| `typeID` | `number\|null` | Transformer type: `1`=PT, `2`=CT |
| `systemSideRatio` | `number\|null` | System-side ratio |
| `monitorSideRatio` | `number\|null` | Monitor-side ratio |
| `frequencyResponse` | `number[]\|null` | Frequency response values |

### CalibrationInfo

| Field | Type | Description |
|-------|------|-------------|
| `timeSkew` | `number\|null` | Time skew correction |
| `offset` | `number\|null` | DC offset correction |
| `ratio` | `number\|null` | Gain ratio correction |
| `mustUseARCal` | `boolean\|null` | Whether AR calibration must be used |
| `applied` | `number[]\|null` | Applied calibration values |
| `recorded` | `number[]\|null` | Recorded calibration values |

### TriggerThresholds

| Field | Type | Description |
|-------|------|-------------|
| `highHigh` | `number\|null` | High-high trigger threshold |
| `high` | `number\|null` | High trigger threshold |
| `low` | `number\|null` | Low trigger threshold |
| `lowLow` | `number\|null` | Low-low trigger threshold |
| `deadBand` | `number\|null` | Trigger dead band |
| `rate` | `number\|null` | Rate-of-change trigger threshold |

---

## Observation Record (`recType: 4`)

A measurement event or data collection snapshot.

| Field | Type | Description |
|-------|------|-------------|
| `recordType` | `string` | Always `"Observation"` |
| `observationName` | `string` | Name of the observation (e.g. `"Steady-state trend"`, `"RMS Variation"`) |
| `timeCreate` | `string\|null` | ISO 8601 timestamp when observation was created |
| `timeStart` | `string\|null` | ISO 8601 timestamp when observation data starts |
| `triggerMethodID` | `number` | How observation was triggered (see Trigger Method IDs below) |
| `timeTriggered` | `string\|null` | ISO 8601 timestamp when trigger occurred |
| `channelTriggerIdx` | `number[]\|null` | Channel indices that triggered the observation |
| `observationSerial` | `number\|null` | Sequential observation number |
| `observationAggregationSerial` | `number\|null` | Aggregation serial number |
| `disturbanceCategoryID` | `string\|null` | GUID hex: IEEE 1159 disturbance category |
| `comments` | `string` | Free-form comments |
| `sagTypeID` | `string\|null` | GUID hex: sag classification type |
| `lsttoLocal` | `number\|null` | LST to Local Time offset |
| `characterization` | `ObservationCharacterization` | Disturbance characterization data |
| `channelInstances` | `ChannelInstance[]` | Array of channel data instances |

### ObservationCharacterization

| Field | Type | Description |
|-------|------|-------------|
| `disturbDirection` | `number\|null` | Disturbance direction |
| `disturbDirectionQuality` | `number\|null` | Quality of direction measurement |
| `minVoltagePU` | `number\|null` | Minimum voltage in per-unit |
| `maxVoltagePU` | `number\|null` | Maximum voltage in per-unit |
| `minVoltage` | `number\|null` | Minimum voltage in volts |
| `maxVoltage` | `number\|null` | Maximum voltage in volts |
| `minCurrent` | `number\|null` | Minimum current in amps |
| `maxCurrent` | `number\|null` | Maximum current in amps |
| `duration` | `number\|null` | Disturbance duration in seconds |

### ChannelInstance

| Field | Type | Description |
|-------|------|-------------|
| `channelDefnIdx` | `number` | Index into the data source's channel definitions |
| `seriesInstances` | `SeriesInstance[]` | Array of series data instances |
| `characterization` | `ChannelCharacterization` | Channel-level characterization |
| `triggerModuleInfo` | `number\|null` | Trigger module information |
| `triggerModuleName` | `string` | Trigger module name |
| `crossTriggerDeviceName` | `string` | Cross-trigger device name |
| `crossTriggerChanIdx` | `number\|null` | Cross-trigger channel index |
| `triggerTypeID` | `number\|null` | Channel trigger type |
| `channelFrequency` | `number\|null` | Measured frequency in Hz |
| `channelGroupID` | `number\|null` | Channel group identifier |

### ChannelCharacterization

| Field | Type | Description |
|-------|------|-------------|
| `magnitude` | `number\|null` | Characterization magnitude |
| `frequency` | `number\|null` | Characterization frequency |

### SeriesInstance

| Field | Type | Description |
|-------|------|-------------|
| `seriesValues` | `number[]\|null` | Array of measurement values. May be truncated with `"..."` sentinel if `maxSeriesValues` was used |
| `seriesBaseQuantity` | `number\|null` | Base quantity for scale normalization (e.g. `34000` for 34kV nominal) |
| `seriesScale` | `number\|null` | Scale factor to convert raw values to physical units |
| `seriesOffset` | `number\|null` | Offset added after scaling |
| `seriesShareChannelIdx` | `number\|null` | If set, references another channel's series (shared data) |
| `seriesShareSeriesIdx` | `number\|null` | If set, references which series within the shared channel |

---

## Quantity Type IDs

These GUIDs identify the type of measurement in a channel definition.

| GUID (hex prefix) | Name | Description |
|-------------------|------|-------------|
| `7067be5e` | WAVEFORM | Oscillographic waveform data |
| `82aff667` | VALUELOG | Time-series RMS or statistical values |
| `c1f00400` | PHASOR | Phasor (magnitude + angle) measurements |
| `91c40400` | RESPONSE | Frequency response / spectrum |
| `72b40400` | FLASH | Flicker (Pst/Plt) measurements |
| `73b40400` | HISTOGRAM | Single-dimension histogram |
| `74b40400` | HISTOGRAM3D | Three-dimension histogram |
| `75b40400` | CPF | Cumulative Probability Function |
| `76b40400` | XY | XY plot data |
| `77b40400` | MAGDUR | Magnitude-duration table |
| `78b40400` | XYZ | XYZ plot data |
| `79b40400` | MAGDURTIME | Magnitude-duration with time |
| `7ab40400` | MAGDURCOUNT | Magnitude-duration with counts |

## Value Type IDs

These GUIDs identify what each series in a channel represents.

| GUID (hex prefix) | Name | Description |
|-------------------|------|-------------|
| `62e890c6` | TIME | Time stamp or time offset |
| `98aff667` | VAL | Measured value |
| `99aff667` | MIN | Minimum value |
| `9aaff667` | MAX | Maximum value |
| `9baff667` | AVG | Average value |
| `9caff667` | INST | Instantaneous value |
| `9daff667` | PHASEANGLE | Phase angle |
| `63e890c6` | AREA | Area under curve |
| `64e890c6` | LATITUDE | Geographic latitude |
| `65e890c6` | LONGITUDE | Geographic longitude |
| `66e890c6` | DURATION | Time duration |
| `67e890c6` | POLARITY | Polarity indicator |
| `68e890c6` | ELLIPSE | Ellipse parameters |
| `69e890c6` | BINID | Histogram bin identifier |
| `6ae890c6` | BINHIGH | Histogram bin high edge |
| `6be890c6` | BINLOW | Histogram bin low edge |
| `6ce890c6` | COUNT | Count / frequency |
| `6de890c6` | TRANSITION | Transition count |
| `6ee890c6` | PROB | Probability |
| `6fe890c6` | INTERVAL | Time interval |
| `70e890c6` | STATUS | Status flags |
| `71e890c6` | RMS | RMS value |

## Phase IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | NONE | No phase |
| 1 | AN | Phase A-N |
| 2 | BN | Phase B-N |
| 3 | CN | Phase C-N |
| 4 | NG | Neutral-Ground |
| 5 | AB | Phase A-B |
| 6 | BC | Phase B-C |
| 7 | CA | Phase C-A |
| 8 | RES | Residual |
| 9 | NET | Net |
| 10 | TOTAL | Total |
| 11 | LNAVE | Line-to-neutral average |
| 12 | LLAVE | Line-to-line average |
| 13 | WORST | Worst phase |
| 14 | PLUS | Positive sequence |
| 15 | MINUS | Negative sequence |

## Quantity Measured IDs

| ID | Name |
|----|------|
| 0 | NONE |
| 1 | VOLTAGE |
| 2 | CURRENT |
| 3 | POWER |
| 4 | ENERGY |
| 5 | FREQUENCY |
| 6 | DISTORTION |
| 7 | FLICKER |
| 8 | UNBALANCE |
| 9 | TEMPERATURE |
| 10 | PRESSURE |
| 11 | HARMONIC |
| 12 | WHOLEAVEHARMONIC |
| 13 | INTERHARMONIC |
| 14 | RIPOPLE |
| 15 | MAINSIGNALING |
| 16 | CAPACITORSWITCHINGTRANSIENT |
| 17 | NOTCH |
| 18 | BURST |
| 19 | DERIVATIVE |
| 20 | IMPEDANCE |

## Quantity Characteristic IDs

| GUID (hex prefix) | Name | Description |
|-------------------|------|-------------|
| `81aff667` | INSTANTANEOUS | Instantaneous value |
| `82aff667` | RMS | RMS value |
| `83aff667` | PEAK | Peak value |
| `84aff667` | CREST | Crest factor |
| `85aff667` | FACTOR | Form factor |
| `86aff667` | THD | Total Harmonic Distortion |
| `87aff667` | THD_R | THD relative |
| `88aff667` | TIF | Telephone Interference Factor |
| `89aff667` | IFACTOR | I-factor |
| `8aaff667` | CRESTFACTOR | Crest factor |
| `8baff667` | CAPACITOR | Capacitor switching |
| `8caff667` | IMPEDANCE | Impedance |
| `8daff667` | SLI | System Linear Impedance |
| `8eaff667` | MSI | Motor Starting Impedance |
| `8faff667` |ari | Arcing |
| `90aff667` | SUPERVISORY | Supervisory |
| `91aff667` | FAST_TRANSIENT | Fast transient |
| `92aff667` | OTHER | Other |
| `93aff667` | POSITIVESEQUENCE | Positive sequence |
| `94aff667` | NEGATIVESEQUENCE | Negative sequence |
| `95aff667` | ZEROSEQUENCE | Zero sequence |
| `96aff667` | POWERRATIO | Power ratio |
| `97aff667` | POWERRATIO_PQ | Power quality ratio |
| `98aff667` | REALPOWER | Real power (P) |
| `99aff667` | REACTIVEPOWER | Reactive power (Q) |
| `9aaff667` | APPARENTPOWER | Apparent power (S) |
| `9baff667` | POWERFACTOR | Power factor |
| `9caff667` | DISPLACEMENTPF | Displacement power factor |
| `9daff667` | TRUEPF | True power factor |
| `9eaff667` | FREQUENCY | Frequency |
| `9faff667` | PST | Short-term flicker |
| `a0aff667` | PLT | Long-term flicker |
| `a1aff667` | FLICKERSEVERITY | Flicker severity |
| `a2aff667` | ENERGYREAL | Real energy |
| `a3aff667` | ENERGYREACTIVE | Reactive energy |
| `a4aff667` | ENERGYAPPARENT | Apparent energy |

## Storage Method IDs

| ID | Name | Description |
|----|------|-------------|
| 1 | VALUES | Direct values |
| 2 | SCALED | Values with scale/offset applied |
| 4 | INCREMENT | Incremental encoding (count, rate pairs) |

## Trigger Method IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | NONE | Not triggered |
| 1 | CHANNEL | Triggered by channel threshold |
| 2 | INTERNAL | Internally triggered |
| 3 | EXTERNAL | Externally triggered |
| 4 | PERIODIC_STATS | Periodic statistics |

## Trigger Type IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | NONE | No trigger |
| 1 | LOW | Low threshold |
| 2 | HIGH | High threshold |
| 3 | RATE | Rate of change |
| 4 | SHAPE | Waveform shape |
| 5 | OTHER | Other trigger |

## Physical Connection IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | SINGLE_PHASE | Single phase |
| 1 | 2ELEMENT_1PHASE | 2-element, 1-phase |
| 2 | 3WATTEMETER | 3-wattmeter |
| 3 | 2WATTEMETER | 2-wattmeter |
| 4 | 3ELEMENT_3PHASE | 3-element, 3-phase |
| 5 | 3ELEMENT_2PHASE | 3-element, 2-phase |
| 6 | 2ELEMENT_2PHASE | 2-element, 2-phase |

## Compression Style IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | NONE | No compression |
| 1 | TOTALFILE | Entire file compressed (deprecated) |
| 2 | RECORDLEVEL | Each record body compressed individually |

## Compression Algorithm IDs

| ID | Name | Description |
|----|------|-------------|
| 0 | NONE | No compression |
| 1 | ZLIB | ZLIB/deflate compression |
| 64 | PKZIPCL | PKZIP compression library (deprecated) |
