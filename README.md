# xAODPerfTools -- Analyzing Jets and Triggers

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Installing](#installing)
  - [Dependencies](#dependencies)
- [Quick Start](#quick-start)
- [Issues](#issues)
- [Authors](#authors)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->


## Installing
The last stable analysis base used is **2.3.18**. To install,
```bash
mkdir myRootCore && cd $_
rcSetup Base,2.3.18
git clone https://github.com/UCATLAS/xAODAnaHelpers
cd xAODAnaHelpers && git checkout 00-03-12 && cd ../
python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.18

git clone https://github.com/UCATLAS/xAODPerfTools
rc checkout xAODPerfTools/packages.txt
cp xAODPerfTools/Makefile.RootCore.xAODMetaDataCnv xAODMetaDataCnv/cmt/Makefile.RootCore

rc find_packages
rc compile
```

### Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)

## Quick Start

```
xAH_run.py --nevents=1000 --config xAODPerfTools/data/jetSelect.json /share/t3data3/kratsg/xAODs/mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.AOD.e3698_s2608_s2183_r6630_r6264_tid05419191_00/AOD.05419191._000003.pool.root.1
```

## Issues

If you get a segfault due to `xAOD::CutBookkeeper_v1::nAcceptedEvents`, then set `m_useMetaData` to `false` instead of `true` in the config file.

## Authors
- [David Miller](https://github.com/fizisist)
- [Giordon Stark](https://github.com/kratsg)
