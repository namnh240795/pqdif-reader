# pqdif-reader

Node.js library for reading, writing, and validating [PQDIF](https://standards.ieee.org/standard/1159_3-2019.html) (Power Quality Data Interchange Format) files per IEEE Std. 1159.3.

PQDIF is a binary file format used to exchange voltage, current, power, and energy measurements between software applications in the power quality domain. This library handles parsing, decompression (ZLIB), validation, and export to JSON, CSV, and XML.

## Install

```bash
npm install pqdif-reader
```

## Quick Start

```js
const { PqFile } = require('pqdif-reader');

const file = new PqFile();
file.read('path/to/measurement.pqd');

console.log('Records:', file.recordHolders.length);
console.log('Data Sources:', file.getDataSourceNames());
console.log('Time Range:', file.getTimeStartMin(), 'to', file.getTimeStartMax());

// Export to JSON
const json = file.getJSON();
require('fs').writeFileSync('output.json', JSON.stringify(json, null, 2));

// Export to CSV
const csv = file.toCSV();
require('fs').writeFileSync('output.csv', csv);

// Export to XML
const xml = file.getXML();
require('fs').writeFileSync('output.xml', xml);
```

## API

### `PqFile`

Main class for reading and working with PQDIF files.

#### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `read(fileName)` | `boolean` | Read a PQDIF file from disk |
| `save(fileName, removePI?, newNameDS?)` | `boolean` | Write records to a new PQDIF file |
| `getJSON(maxSeriesValues?)` | `Object` | Export entire file as JSON |
| `toCSV(maxValuesPerSeries?)` | `string` | Export observation data as CSV |
| `toSummaryCSV()` | `string` | Export record summary as CSV |
| `getXML()` | `string` | Export entire file as XML |
| `getDataSourceNames()` | `string[]` | Sorted list of data source names |
| `getTimeStartMin()` | `Date\|null` | Earliest observation start time |
| `getTimeStartMax()` | `Date\|null` | Latest observation start time |
| `getNominalVoltage(recordHolder)` | `number\|null` | Nominal voltage for an observation |
| `getTimeStampLST(recordHolder, date)` | `Date` | Convert UTC to Local Standard Time |
| `getTimeStampLocal(recordHolder, date)` | `Date` | Convert UTC to Local Time |

#### Properties

| Property | Type | Description |
|----------|------|-------------|
| `fileName` | `string` | Path to the PQDIF file |
| `fileRead` | `boolean` | Whether the file has been read |
| `fileOK` | `boolean` | Whether the file was read successfully |
| `recordHolders` | `RecordHolder[]` | All parsed records |
| `loggerApplication` | `Logger` | Application-level log events |
| `loggerCompliance` | `Logger` | IEEE compliance log events |

### Record Types

Records are accessed via `file.recordHolders`. Each `RecordHolder` has:

| Property | Type | Description |
|----------|------|-------------|
| `recIndex` | `number` | Record position in file |
| `recType` | `number` | 1=Container, 2=DataSource, 3=MonitorSettings, 4=Observation |
| `record` | `Object` | The parsed record data |
| `dataSource` | `DataSource\|null` | Linked data source (for observations) |
| `monitorSettings` | `MonitorSettings\|null` | Linked monitor settings |

#### Container Record

```js
const container = file.recordHolders[0].record;
container.fileName      // Original file name
container.creation      // Date object
container.title         // File title
container.author        // Author
container.owner         // Owner
container.application   // Application that created the file
container.compressionStyleID   // 0=none, 2=record-level
container.compressionAlgorithmID  // 0=none, 1=zlib
```

#### DataSource Record

```js
const ds = file.recordHolders[1].record;
ds.nameDS               // Data source name
ds.vendorID             // Vendor GUID (Buffer)
ds.equipmentID          // Equipment GUID (Buffer)
ds.channelDefns         // Array of ChannelDefinition
ds.channelDefns[0].channelName   // e.g. "Waveform VAN"
ds.channelDefns[0].quantityTypeID // Quantity type GUID
ds.channelDefns[0].seriesDefns   // Array of SeriesDefinition
```

#### Observation Record

```js
const obs = file.recordHolders[3].record;
obs.observationName     // e.g. "Steady-state trend"
obs.timeStart           // Date object
obs.timeCreate          // Date object
obs.channelInstances    // Array of ChannelInstance

const ci = obs.channelInstances[0];
ci.channelDefnIdx       // Index into data source channel definitions
ci.seriesInstances      // Array of SeriesInstance

const si = ci.seriesInstances[0];
si.seriesValues         // TypedArray of values (Uint16Array, Float64Array, etc.)
si.seriesBaseQuantity   // Base quantity for scaling
si.seriesScale          // Scale factor
si.seriesOffset         // Offset
```

### Export Options

#### JSON Export

```js
const json = file.getJSON();           // Full export
const json = file.getJSON(100);        // Max 100 values per series (truncates waveforms)
```

The JSON output contains:

```json
{
  "fileName": "measurement.pqd",
  "records": [
    {
      "recIndex": 0,
      "recType": 1,
      "record": {
        "recordType": "Container",
        "versionInfo": [1, 5, 1, 5],
        "fileName": "...",
        "creation": "1999-07-16T21:27:05.000Z",
        "compressionStyleID": 2,
        "compressionAlgorithmID": 1,
        ...
      }
    },
    {
      "recIndex": 1,
      "recType": 2,
      "record": {
        "recordType": "DataSource",
        "name": "PQDIF Convert",
        "channelDefinitions": [
          {
            "channelName": "Waveform VAN",
            "quantityTypeID": "7067be5e...",
            "seriesDefinitions": [...]
          }
        ]
      }
    },
    {
      "recIndex": 3,
      "recType": 4,
      "record": {
        "recordType": "Observation",
        "observationName": "Steady-state trend",
        "timeStart": "1999-06-01T01:06:20.000Z",
        "channelInstances": [
          {
            "channelDefnIdx": 29,
            "seriesInstances": [
              {
                "seriesValues": [31486, 31528, ...],
                "seriesBaseQuantity": 34000,
                "seriesScale": 1.104
              }
            ]
          }
        ]
      }
    }
  ]
}
```

#### JSON Field Reference

| Record Type | Key Fields |
|-------------|-----------|
| **Container** | `versionInfo`, `fileName`, `creation`, `compressionStyleID`, `compressionAlgorithmID`, `language`, `title`, `author`, `owner`, `application`, `contact` |
| **DataSource** | `name`, `dataSourceTypeID`, `vendorID`, `equipmentID`, `serialNumber`, `channelDefinitions[]`, `customSourceInfo` |
| **MonitorSettings** | `effective`, `useCalibration`, `useTransducer`, `nominalFrequency`, `channelSettings[]` |
| **Observation** | `observationName`, `timeStart`, `timeCreate`, `triggerMethodID`, `channelInstances[]`, `characterization` |

**ChannelDefinition:** `channelName`, `phaseID`, `quantityTypeID`, `quantityMeasuredID`, `seriesDefinitions[]`

**SeriesDefinition:** `valueTypeID`, `quantityUnitsID`, `quantityCharacteristicID`, `storageMethodID`, `seriesNominalQuantity`

**ChannelInstance:** `channelDefnIdx`, `seriesInstances[]`, `channelFrequency`, `channelGroupID`

**SeriesInstance:** `seriesValues[]`, `seriesBaseQuantity`, `seriesScale`, `seriesOffset`, `seriesShareChannelIdx`, `seriesShareSeriesIdx`

**ChannelSetting:** `channelDefnIdx`, `triggerTypeID`, `fullScale`, `noiseFloor`, `transformer`, `calibration`, `triggerThresholds`

GUIDs are hex strings. Dates are ISO 8601. Typed arrays are plain number arrays. Full field documentation: [docs/json-fields.md](docs/json-fields.md)

#### CSV Export

```js
// Full observation data
const csv = file.toCSV();
const csv = file.toCSV(100);  // Max 100 values per series

// Summary of all records
const summary = file.toSummaryCSV();
```

Data CSV columns: `ObservationName, TimeCreate, TimeStart, ChannelDefnIdx, ChannelName, PhaseID, QuantityType, SeriesIndex, ValueType, StorageMethod, Values`

Summary CSV columns: `RecordIndex, RecordType, Name, TimeStart, ChannelCount, SeriesCount, ValueCount`

#### XML Export

```js
const xml = file.getXML();
```

### Logger

```js
const { Logger, LogLevels } = require('pqdif-reader');

// Check compliance issues
for (const log of file.loggerCompliance.getLogEventList()) {
  console.log(`[${LogLevels[log.level]}] ${log.message}`);
}

// Get as XML
const logXml = file.loggerCompliance.getLogXML();
```

### Constants

All PQDIF standard constants are available:

```js
const { constants } = require('pqdif-reader');

// Physical types
constants.ID_PHYS_TYPE_REAL8      // 41
constants.ID_PHYS_TYPE_TIMESTAMPPQDIF  // 50

// Element types
constants.ID_ELEMENT_TYPE_COLLECTION  // 1
constants.ID_ELEMENT_TYPE_SCALAR      // 2
constants.ID_ELEMENT_TYPE_VECTOR      // 3

// Compression
constants.ID_COMP_STYLE_RECORDLEVEL  // 2
constants.ID_COMP_ALG_ZLIB           // 1

// Tag GUIDs (16-byte Buffers)
constants.tagContainer
constants.tagRecDataSource
constants.tagRecObservation
constants.tagChannelDefns
constants.tagSeriesValues

// ID GUIDs
constants.ID_VENDOR_ELECTROTEK
constants.ID_QT_WAVEFORM
constants.ID_QC_RMS
constants.ID_SERIES_VALUE_TYPE_VAL
```

### PqdifInfo Utility

```js
const { PqdifInfo } = require('pqdif-reader');

PqdifInfo.getTypeSize(41)          // 8 (REAL8 = 8 bytes)
PqdifInfo.getTypeSize(50)          // 12 (TIMESTAMPPQDIF = 12 bytes)
PqdifInfo.getTagName(tagBuffer)    // "tagContainer"
PqdifInfo.guidEquals(a, b)         // Compare two GUID buffers
PqdifInfo.guidFromString('89738606-f1c3-11cf-9d89-0080c72e70a3')  // Buffer
```

## PQDIF File Structure

A PQDIF file contains a sequence of records:

```
File
├── Container Record (always first)
│   ├── Version info, filename, creation date
│   ├── Compression settings
│   └── Contact/metadata
├── DataSource Record(s)
│   ├── Vendor, equipment, serial number
│   ├── Channel definitions (37 channels typical)
│   │   ├── Channel name, phase, quantity type
│   │   └── Series definitions (TIME, VAL, etc.)
│   └── Custom source info
├── MonitorSettings Record(s) (optional)
│   ├── Calibration, transducer settings
│   ├── Trigger thresholds
│   └── Channel settings
└── Observation Record(s)
    ├── Timestamps, trigger method
    └── Channel instances
        └── Series instances
            ├── Series values (Uint16Array, Float64Array, etc.)
            ├── Scale, offset, base quantity
            └── Shared series references
```

## Supported Data Types

| Measurement | PQDIF Type | Typical Channels |
|-------------|-----------|-----------------|
| Waveforms | `ID_QT_WAVEFORM` | VAN, VBN, VCN, IAN, IBN, ICN |
| RMS Values | `ID_QT_VALUELOG` | RMS Voltage, RMS Current |
| Phasors | `ID_QT_PHASOR` | Voltage/Current phasors |
| Harmonics | `ID_QT_RESPONSE` | THD, individual harmonics |
| Flicker | `ID_QT_FLASH` | Pst, Plt |
| Magnitude-Duration | `ID_QT_MAGDUR` | Sag/Swell/Interrupt tables |

## Compression

PQDIF supports optional ZLIB compression at the record level. This library handles decompression automatically:

```js
// Compression is transparent - just read the file
const file = new PqFile();
file.read('compressed.pqd');

// Check compression settings
const container = file.recordHolders[0].record;
console.log('Compression:', container.compressionStyleID === 2 ? 'Record-level ZLIB' : 'None');
```

## Writing PQDIF Files

```js
const { PqFile, Writer } = require('pqdif-reader');

// Use the Writer to create example files
const writer = new Writer();
writer.saveExampleEventWaveforms('waveforms.pqd');
writer.saveExampleEventRms('rms.pqd');
writer.saveExampleEventWaveformsRms('combined.pqd');
```

## HTML Viewer

An included HTML viewer visualizes PQDIF data as charts:

```bash
cd examples
python3 -m http.server 8080
# Open http://localhost:8080/index.html
```

Features: observation/channel selectors, waveform charts, channel type summaries, observation timeline, records table. Supports drag-and-drop JSON file loading.

## Requirements

- Node.js >= 14
- No native dependencies (pure JavaScript + pako for zlib)

## License

Apache-2.0 - See [LICENSE](LICENSE) for details.

Based on the [IEEE PQDIF reference implementation](https://opensource.ieee.org/pqdif/pqdifnet).
