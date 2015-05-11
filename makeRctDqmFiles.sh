#!/bin/bash

# Files created by this script:
#
#        L1TRCToffline_cff.py  in python/
# Rct_LUTconfiguration_cff.py  in python/
#           testRCToffline.py  in   test/
#                   lsfJob.csh in   test/

# Script parameters
nArguments=3
badArgumentExit=64

# Configuration parameters
nEvents=60000
nFiles=50
#globalString=Run2011A
globalString=Commissioning2015
#pathString=HT/RAW/v1
pathString=MinimumBias/RAW/v1
#pathString=L1Accept/RAW/v3
key=EEG_EHSUMS_TAU3_DECO_25_FALLGR09_FULLECAL
# pathString=TestEnables/RAW/v1   # ONLY FOR TESTS

if [ $# -lt "$nArguments" ]
then
  echo "Syntax: ./makeRctDqmPlots.sh <run#> <date> <time>"
  exit $badArgumentExit
fi

# Input parameters
run=$1
date=$2
startTime=$3
timezone=GMT

runStart=${run:0:3}
runEnd=${run:3}

if [ ! -d $CMSSW_BASE/src/DQM/L1TMonitor/test/run$run ]
then
  mkdir $CMSSW_BASE/src/DQM/L1TMonitor/test/run$run
fi

echo $date $startTime $timezone > $CMSSW_BASE/src/DQM/L1TMonitor/test/run$run/description.txt

cat << EOF > "$CMSSW_BASE/src/DQM/L1TMonitor/python/L1TRCToffline_${run}_cff.py"
import FWCore.ParameterSet.Config as cms

#from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *

#add'n
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *
#from Configuration.StandardSequences.RawToDigi_Repacked_cff import *

#emulator/comparator
from L1Trigger.HardwareValidation.L1HardwareValidation_cff import *
from L1Trigger.Configuration.L1Config_cff import *

#for LUTs
from DQM.L1TMonitor.Rct_LUTconfiguration_${run}_cff import *

# For the GT
from L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff import *

#dqm
rctEmulDigis = cms.EDProducer("L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),
    useDebugTpgScales = cms.bool(False),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    queryDelayInLS = cms.uint32(10),
    queryIntervalInLS = cms.uint32(100),
    getFedsFromOmds = cms.bool(False),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")),
    BunchCrossings = cms.vint32(0),
    conditionsLabel = cms.string("")
)

l1tderct = cms.EDAnalyzer("L1TdeRCT",
    rctSourceData = cms.InputTag("l1GctHwDigis"),
    HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT/'),
    outputFile = cms.untracked.string('./run$run.root'),
    verbose = cms.untracked.bool(False),
    DQMStore = cms.untracked.bool(True),
    singlechannelhistos = cms.untracked.bool(False),
    ecalTPGData = cms.InputTag("",""),
    rctSourceEmul = cms.InputTag("rctDigis"),
    disableROOToutput = cms.untracked.bool(False),
    hcalTPGData = cms.InputTag(""),
    gtEGAlgoName = cms.string("L1_SingleEG1"),
    doubleThreshold = cms.int32(3),
    gtDigisLabel = cms.InputTag("gtDigis"),
    dumpError = cms.untracked.bool(True),
    filterTriggerType = cms.int32(1)
)

l1tderct.rctSourceData = 'gctDigis'
l1tderct.rctSourceEmul = 'rctEmulDigis'
l1tderct.ecalTPGData = 'ecalDigis:EcalTriggerPrimitives'
l1tderct.hcalTPGData = 'hcalDigis'

l1trct = cms.EDAnalyzer("L1TRCT",
    DQMStore = cms.untracked.bool(True),
    disableROOToutput = cms.untracked.bool(False),
    outputFile = cms.untracked.string('./run$run.root'),
    rctSource = cms.InputTag("l1GctHwDigis","","DQM"),
    verbose = cms.untracked.bool(False),
    filterTriggerType = cms.int32(1)
)

l1trct.rctSource = 'gctDigis'

rctdqm = cms.Sequence(
     cms.SequencePlaceholder("RawToDigi")
    *cms.SequencePlaceholder("rctEmulDigis")
    *cms.SequencePlaceholder("l1trct")
    *cms.SequencePlaceholder("l1tderct")
)
EOF

cat $CMSSW_BASE/src/${key}_cff.py > "$CMSSW_BASE/src/DQM/L1TMonitor/python/Rct_LUTconfiguration_${run}_cff.py"

cat << EOF > "$CMSSW_BASE/src/DQM/L1TMonitor/test/testRCToffline_${run}.py"
import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTofflineTEST")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR10_P_V2::All'
#process.GlobalTag.globaltag = 'GR10_P_V3::All'
#process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
#process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = 'GR_H_V45'

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.DQMStore = cms.Service("DQMStore")

process.load("DQM/L1TMonitor/L1TRCToffline_${run}_cff")

process.qTester = cms.EDAnalyzer("QualityTester",
    qtList          = cms.untracked.FileInPath('DQM/L1TMonitor/data/L1TdeRCTQualityTests.xml'),
    prescaleFactor  = cms.untracked.int32     (1),
    testInEventloop = cms.untracked.bool      (True),
    verboseQT       = cms.untracked.bool      (True),
)

process.p = cms.Path(process.rctdqm*process.qTester)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32($nEvents)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
EOF

/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select ls /eos/cms/store/data/$globalString/$pathString/000/$runStart/$runEnd/00000 |
head -n $nFiles |
awk -v globalTemp=$globalString -v pathTemp=$pathString -v runStartTemp=$runStart -v runEndTemp=$runEnd '{print "        '\''root://eoscms//eos/cms/store/data/" globalTemp "/" pathTemp "/000/" runStartTemp "/" runEndTemp "/00000/" $1 "'\'',"}' |
sed '$s/.$//' >> "$CMSSW_BASE/src/DQM/L1TMonitor/test/testRCToffline_${run}.py"

cat << EOF >> "$CMSSW_BASE/src/DQM/L1TMonitor/test/testRCToffline_${run}.py"
    )
)
EOF
 
cat << EOF > "$CMSSW_BASE/src/DQM/L1TMonitor/test/lsfJob.csh"
#!/bin/csh
cd $CMSSW_BASE/src/DQM/L1TMonitor/python
scramv1 b
cd $CMSSW_BASE/src/DQM/L1TMonitor/test
setenv SCRAM_ARCH slc5_amd64_gcc462
cmsenv

cmsRun testRCToffline_${run}.py
EOF

chmod +x lsfJob.csh

#bsub -q 8nh < lsfJob.csh
