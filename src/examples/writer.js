'use strict';

const { FlatFileController } = require('../persistence');
const { dateToPqdifTimestamp } = require('../validator/utilities');
const {
  ID_DS_TYPE_MEASURE, ID_VENDOR_IEEE, ID_EQUIP_NONE,
  ID_COMP_STYLE_RECORDLEVEL, ID_COMP_ALG_ZLIB,
  ID_PHASE_AN, ID_PHASE_BN, ID_PHASE_CN,
  ID_QM_VOLTAGE, ID_QM_CURRENT,
  ID_QT_WAVEFORM, ID_QT_PHASOR,
  ID_QU_SECONDS, ID_QU_VOLTS, ID_QU_AMPS,
  ID_SERIES_VALUE_TYPE_TIME, ID_SERIES_VALUE_TYPE_VAL,
  ID_QC_INSTANTANEOUS, ID_QC_RMS,
  ID_SERIES_METHOD_VALUES,
  ID_TRIGGER_METH_EXTERNAL,
} = require('../constants');

/**
 * Creates Example PQDIF Files
 * Ported from PQDIF.Examples.Writer.cs
 */
class Writer {
  constructor() {
    this.SQRT2 = 1.41421356237;
  }

  saveExampleEventWaveforms(fileName) { return this._saveWaveformsOnly(fileName, 'Power Quality Instrument 1'); }
  saveExampleEventRms(fileName) { return this._saveRmsOnly(fileName, 'Power Quality Instrument 2'); }
  saveExampleEventWaveformsRms(fileName) { return this._saveWaveformsAndRms(fileName, 'Power Quality Instrument 3'); }

  _saveWaveformsOnly(fileName, instrumentName) {
    try {
      const NV = 13279.0561914;
      const NF = 60;
      const dtSettings = new Date(Date.UTC(2022, 5, 1, 0, 0, 0));
      const dtCreated = new Date(Date.UTC(2022, 6, 15, 0, 0, 0));

      const ctrl = new FlatFileController();
      ctrl.setFileName(fileName);
      ctrl.setCompressionStyle(ID_COMP_STYLE_RECORDLEVEL);
      ctrl.setCompressionAlgorithm(ID_COMP_ALG_ZLIB);

      const tc = dateToPqdifTimestamp(dtCreated);
      ctrl.createContainerRecord(fileName, [1,5,1,5], tc, 1,5,1,5);
      ctrl.createContainerRecordWithMeta('English', 'Example PQDIF File with Observation Events with Waveforms Samples', 'Example PQDIF File', 'PQDIF Authors', '', '', '', '', '', '', 'Copyright 2023 PQDIF Authors', '', '');

      ctrl.createDataSourceRecord(1, ID_DS_TYPE_MEASURE, ID_VENDOR_IEEE, ID_EQUIP_NONE, '', '', instrumentName, '', '', '');
      const ds = ctrl.getRecord(1);
      if (!ds) return false;

      const chVa = ds.addChannelDefn2('V Waveform A', ID_PHASE_AN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chVb = ds.addChannelDefn2('V Waveform B', ID_PHASE_BN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chVc = ds.addChannelDefn2('V Waveform C', ID_PHASE_CN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chIa = ds.addChannelDefn2('I Waveform A', ID_PHASE_AN, ID_QM_CURRENT, ID_QT_WAVEFORM);
      const chIb = ds.addChannelDefn2('I Waveform B', ID_PHASE_BN, ID_QM_CURRENT, ID_QT_WAVEFORM);
      const chIc = ds.addChannelDefn2('I Waveform C', ID_PHASE_CN, ID_QM_CURRENT, ID_QT_WAVEFORM);

      const sdVaT = ds.addSeriesDefn2(chVa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVaV = ds.addSeriesDefn2(chVa, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVbV = ds.addSeriesDefn2(chVb, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVcV = ds.addSeriesDefn2(chVc, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);

      ds.setSeriesDefnNominal(chVa, sdVaV, this.SQRT2 * NV);
      ds.setSeriesDefnNominal(chVb, sdVbV, this.SQRT2 * NV);
      ds.setSeriesDefnNominal(chVc, sdVcV, this.SQRT2 * NV);

      ctrl.createMonitorSettingsRecord(2);
      const sett = ctrl.getRecord(2);
      if (sett) {
        const ts = dateToPqdifTimestamp(dtSettings);
        sett.setInfo(ts, ts, ts, false, false);
        sett.setNominalFrequency(NF);
      }

      this._addWaveformObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, tc);

      ctrl.writeNew();
      return true;
    } catch (e) { return false; }
  }

  _saveRmsOnly(fileName, instrumentName) {
    try {
      const NV = 13279.0561914;
      const NF = 60;
      const dtSettings = new Date(Date.UTC(2022, 5, 1, 0, 0, 0));
      const dtCreated = new Date(Date.UTC(2022, 6, 15, 0, 0, 0));

      const ctrl = new FlatFileController();
      ctrl.setFileName(fileName);
      ctrl.setCompressionStyle(ID_COMP_STYLE_RECORDLEVEL);
      ctrl.setCompressionAlgorithm(ID_COMP_ALG_ZLIB);

      const tc = dateToPqdifTimestamp(dtCreated);
      ctrl.createContainerRecord(fileName, [1,5,1,5], tc, 1,5,1,5);
      ctrl.createContainerRecordWithMeta('English', 'Example PQDIF File with Observation Events with RMS Values', 'Example PQDIF File', 'PQDIF Authors', '', '', '', '', '', '', 'Copyright 2023 PQDIF Authors', '', '');

      ctrl.createDataSourceRecord(1, ID_DS_TYPE_MEASURE, ID_VENDOR_IEEE, ID_EQUIP_NONE, '', '', instrumentName, '', '', '');
      const ds = ctrl.getRecord(1);
      if (!ds) return false;

      const chVa = ds.addChannelDefn2('V RMS A', ID_PHASE_AN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chVb = ds.addChannelDefn2('V RMS B', ID_PHASE_BN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chVc = ds.addChannelDefn2('V RMS C', ID_PHASE_CN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chIa = ds.addChannelDefn2('I RMS A', ID_PHASE_AN, ID_QM_CURRENT, ID_QT_PHASOR);
      const chIb = ds.addChannelDefn2('I RMS B', ID_PHASE_BN, ID_QM_CURRENT, ID_QT_PHASOR);
      const chIc = ds.addChannelDefn2('I RMS C', ID_PHASE_CN, ID_QM_CURRENT, ID_QT_PHASOR);

      ds.addSeriesDefn2(chVa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVaV = ds.addSeriesDefn2(chVa, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVbV = ds.addSeriesDefn2(chVb, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVcV = ds.addSeriesDefn2(chVc, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);

      ds.setSeriesDefnNominal(chVa, sdVaV, NV);
      ds.setSeriesDefnNominal(chVb, sdVbV, NV);
      ds.setSeriesDefnNominal(chVc, sdVcV, NV);

      ctrl.createMonitorSettingsRecord(2);
      const sett = ctrl.getRecord(2);
      if (sett) {
        const ts = dateToPqdifTimestamp(dtSettings);
        sett.setInfo(ts, ts, ts, false, false);
        sett.setNominalFrequency(NF);
      }

      this._addRmsObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, tc);

      ctrl.writeNew();
      return true;
    } catch (e) { return false; }
  }

  _saveWaveformsAndRms(fileName, instrumentName) {
    try {
      const NV = 13279.0561914;
      const NF = 60;
      const dtSettings = new Date(Date.UTC(2022, 5, 1, 0, 0, 0));
      const dtCreated = new Date(Date.UTC(2022, 6, 15, 0, 0, 0));

      const ctrl = new FlatFileController();
      ctrl.setFileName(fileName);
      ctrl.setCompressionStyle(ID_COMP_STYLE_RECORDLEVEL);
      ctrl.setCompressionAlgorithm(ID_COMP_ALG_ZLIB);

      const tc = dateToPqdifTimestamp(dtCreated);
      ctrl.createContainerRecord(fileName, [1,5,1,5], tc, 1,5,1,5);
      ctrl.createContainerRecordWithMeta('English', 'Example PQDIF File with Observation Events with Waveforms Samples and RMS Values', 'Example PQDIF File', 'PQDIF Authors', '', '', '', '', '', '', 'Copyright 2023 PQDIF Authors', '', '');

      ctrl.createDataSourceRecord(1, ID_DS_TYPE_MEASURE, ID_VENDOR_IEEE, ID_EQUIP_NONE, '', '', instrumentName, '', '', '');
      const ds = ctrl.getRecord(1);
      if (!ds) return false;

      // Waveform channels
      const chVa = ds.addChannelDefn2('V Waveform A', ID_PHASE_AN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chVb = ds.addChannelDefn2('V Waveform B', ID_PHASE_BN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chVc = ds.addChannelDefn2('V Waveform C', ID_PHASE_CN, ID_QM_VOLTAGE, ID_QT_WAVEFORM);
      const chIa = ds.addChannelDefn2('I Waveform A', ID_PHASE_AN, ID_QM_CURRENT, ID_QT_WAVEFORM);
      const chIb = ds.addChannelDefn2('I Waveform B', ID_PHASE_BN, ID_QM_CURRENT, ID_QT_WAVEFORM);
      const chIc = ds.addChannelDefn2('I Waveform C', ID_PHASE_CN, ID_QM_CURRENT, ID_QT_WAVEFORM);

      ds.addSeriesDefn2(chVa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVaV = ds.addSeriesDefn2(chVa, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVbV = ds.addSeriesDefn2(chVb, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      const sdVcV = ds.addSeriesDefn2(chVc, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIa, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIb, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIc, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_INSTANTANEOUS, ID_SERIES_METHOD_VALUES);

      // RMS channels
      const chVaR = ds.addChannelDefn2('V RMS A', ID_PHASE_AN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chVbR = ds.addChannelDefn2('V RMS B', ID_PHASE_BN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chVcR = ds.addChannelDefn2('V RMS C', ID_PHASE_CN, ID_QM_VOLTAGE, ID_QT_PHASOR);
      const chIaR = ds.addChannelDefn2('I RMS A', ID_PHASE_AN, ID_QM_CURRENT, ID_QT_PHASOR);
      const chIbR = ds.addChannelDefn2('I RMS B', ID_PHASE_BN, ID_QM_CURRENT, ID_QT_PHASOR);
      const chIcR = ds.addChannelDefn2('I RMS C', ID_PHASE_CN, ID_QM_CURRENT, ID_QT_PHASOR);

      ds.addSeriesDefn2(chVaR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVaRV = ds.addSeriesDefn2(chVaR, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVbR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVbRV = ds.addSeriesDefn2(chVbR, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chVcR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      const sdVcRV = ds.addSeriesDefn2(chVcR, ID_QU_VOLTS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIaR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIaR, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIbR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIbR, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIcR, ID_QU_SECONDS, ID_SERIES_VALUE_TYPE_TIME, ID_QC_RMS, ID_SERIES_METHOD_VALUES);
      ds.addSeriesDefn2(chIcR, ID_QU_AMPS, ID_SERIES_VALUE_TYPE_VAL, ID_QC_RMS, ID_SERIES_METHOD_VALUES);

      // Nominal voltages
      ds.setSeriesDefnNominal(chVa, sdVaV, this.SQRT2 * NV);
      ds.setSeriesDefnNominal(chVb, sdVbV, this.SQRT2 * NV);
      ds.setSeriesDefnNominal(chVc, sdVcV, this.SQRT2 * NV);
      ds.setSeriesDefnNominal(chVaR, sdVaRV, NV);
      ds.setSeriesDefnNominal(chVbR, sdVbRV, NV);
      ds.setSeriesDefnNominal(chVcR, sdVcRV, NV);

      ctrl.createMonitorSettingsRecord(2);
      const sett = ctrl.getRecord(2);
      if (sett) {
        const ts = dateToPqdifTimestamp(dtSettings);
        sett.setInfo(ts, ts, ts, false, false);
        sett.setNominalFrequency(NF);
      }

      this._addWaveformAndRmsObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, chVaR, chVbR, chVcR, chIaR, chIbR, chIcR, tc);

      ctrl.writeNew();
      return true;
    } catch (e) { return false; }
  }

  _addWaveformObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, tc) {
    const { ObservationRecord } = require('../core');
    for (let i = 0; i < 3; i++) {
      const d = WAVEFORM_DATA[i];
      const ts = dateToPqdifTimestamp(d.t0);
      ctrl.createObservationRecord(3 + i, d.name, tc, ts, ID_TRIGGER_METH_EXTERNAL, tc, 0, []);
      const rec = ctrl.getRecord(3 + i);
      const obs = new ObservationRecord(rec);
      obs.setDataSource(ds);

      const cVa = obs.addChannel(chVa);
      const cVb = obs.addChannel(chVb);
      const cVc = obs.addChannel(chVc);
      const cIa = obs.addChannel(chIa);
      const cIb = obs.addChannel(chIb);
      const cIc = obs.addChannel(chIc);

      const sdT = obs.addSeriesDouble(cVa, d.st.length, d.st);
      obs.addSeriesShared(cVb, cVa, sdT);
      obs.addSeriesShared(cVc, cVa, sdT);
      obs.addSeriesShared(cIa, cVa, sdT);
      obs.addSeriesShared(cIb, cVa, sdT);
      obs.addSeriesShared(cIc, cVa, sdT);

      obs.addSeriesDouble(cVa, d.va.length, d.va);
      obs.addSeriesDouble(cVb, d.vb.length, d.vb);
      obs.addSeriesDouble(cVc, d.vc.length, d.vc);
      obs.addSeriesDouble(cIa, d.ia.length, d.ia);
      obs.addSeriesDouble(cIb, d.ib.length, d.ib);
      obs.addSeriesDouble(cIc, d.ic.length, d.ic);
    }
  }

  _addRmsObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, tc) {
    const { ObservationRecord } = require('../core');
    for (let i = 0; i < 3; i++) {
      const d = RMS_DATA[i];
      const ts = dateToPqdifTimestamp(d.t0);
      ctrl.createObservationRecord(3 + i, d.name, tc, ts, ID_TRIGGER_METH_EXTERNAL, tc, 0, []);
      const rec = ctrl.getRecord(3 + i);
      const obs = new ObservationRecord(rec);
      obs.setDataSource(ds);

      const cVa = obs.addChannel(chVa);
      const cVb = obs.addChannel(chVb);
      const cVc = obs.addChannel(chVc);
      const cIa = obs.addChannel(chIa);
      const cIb = obs.addChannel(chIb);
      const cIc = obs.addChannel(chIc);

      const sdT = obs.addSeriesDouble(cVa, d.st.length, d.st);
      obs.addSeriesShared(cVb, cVa, sdT);
      obs.addSeriesShared(cVc, cVa, sdT);
      obs.addSeriesShared(cIa, cVa, sdT);
      obs.addSeriesShared(cIb, cVa, sdT);
      obs.addSeriesShared(cIc, cVa, sdT);

      obs.addSeriesDouble(cVa, d.va.length, d.va);
      obs.addSeriesDouble(cVb, d.vb.length, d.vb);
      obs.addSeriesDouble(cVc, d.vc.length, d.vc);
      obs.addSeriesDouble(cIa, d.ia.length, d.ia);
      obs.addSeriesDouble(cIb, d.ib.length, d.ib);
      obs.addSeriesDouble(cIc, d.ic.length, d.ic);
    }
  }

  _addWaveformAndRmsObservations(ctrl, ds, chVa, chVb, chVc, chIa, chIb, chIc, chVaR, chVbR, chVcR, chIaR, chIbR, chIcR, tc) {
    const { ObservationRecord } = require('../core');
    for (let i = 0; i < 3; i++) {
      const wd = WAVEFORM_DATA[i];
      const rd = RMS_DATA[i];
      const ts = dateToPqdifTimestamp(wd.t0);
      ctrl.createObservationRecord(3 + i, wd.name, tc, ts, ID_TRIGGER_METH_EXTERNAL, tc, 0, []);
      const rec = ctrl.getRecord(3 + i);
      const obs = new ObservationRecord(rec);
      obs.setDataSource(ds);

      // Waveform channels
      const cVa = obs.addChannel(chVa);
      const cVb = obs.addChannel(chVb);
      const cVc = obs.addChannel(chVc);
      const cIa = obs.addChannel(chIa);
      const cIb = obs.addChannel(chIb);
      const cIc = obs.addChannel(chIc);

      const sdT = obs.addSeriesDouble(cVa, wd.st.length, wd.st);
      obs.addSeriesShared(cVb, cVa, sdT);
      obs.addSeriesShared(cVc, cVa, sdT);
      obs.addSeriesShared(cIa, cVa, sdT);
      obs.addSeriesShared(cIb, cVa, sdT);
      obs.addSeriesShared(cIc, cVa, sdT);

      obs.addSeriesDouble(cVa, wd.va.length, wd.va);
      obs.addSeriesDouble(cVb, wd.vb.length, wd.vb);
      obs.addSeriesDouble(cVc, wd.vc.length, wd.vc);
      obs.addSeriesDouble(cIa, wd.ia.length, wd.ia);
      obs.addSeriesDouble(cIb, wd.ib.length, wd.ib);
      obs.addSeriesDouble(cIc, wd.ic.length, wd.ic);

      // RMS channels
      const cVaR = obs.addChannel(chVaR);
      const cVbR = obs.addChannel(chVbR);
      const cVcR = obs.addChannel(chVcR);
      const cIaR = obs.addChannel(chIaR);
      const cIbR = obs.addChannel(chIbR);
      const cIcR = obs.addChannel(chIcR);

      const sdTR = obs.addSeriesDouble(cVaR, wd.st.length, wd.st);
      obs.addSeriesShared(cVbR, cVaR, sdTR);
      obs.addSeriesShared(cVcR, cVaR, sdTR);
      obs.addSeriesShared(cIaR, cVaR, sdTR);
      obs.addSeriesShared(cIbR, cVaR, sdTR);
      obs.addSeriesShared(cIcR, cVaR, sdTR);

      obs.addSeriesDouble(cVaR, rd.va.length, rd.va);
      obs.addSeriesDouble(cVbR, rd.vb.length, rd.vb);
      obs.addSeriesDouble(cVcR, rd.vc.length, rd.vc);
      obs.addSeriesDouble(cIaR, rd.ia.length, rd.ia);
      obs.addSeriesDouble(cIbR, rd.ib.length, rd.ib);
      obs.addSeriesDouble(cIcR, rd.ic.length, rd.ic);
    }
  }
}

const { WAVEFORM_DATA, RMS_DATA } = require('./writerData');

module.exports = Writer;
