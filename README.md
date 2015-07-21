# xAODPerfTools -- Analyzing Jets and Triggers

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Installing](#installing)
  - [Dependencies](#dependencies)
- [Functionality Included](#functionality-included)
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

rc find_packages
rc compile
```

### Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)

## Functionality Included

## Authors
- [David Miller](https://github.com/fizisist)
- [Giordon Stark](https://github.com/kratsg)
