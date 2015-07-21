# xAODPerfTools -- Analyzing Jets and Triggers

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Installing](#installing)
  - [Dependencies](#dependencies)
- [Functionality Included](#functionality-included)
    - [[Audit.cxx](TheAccountant/Audit.h)](#auditcxxtheaccountantaudith)
    - [[Preselect.cxx](TheAccountant/Preselect.h)](#preselectcxxtheaccountantpreselecth)
    - [[Report.cxx](TheAccountant/Report.h)](#reportcxxtheaccountantreporth)
    - [[CookTheBooks.py](scripts/CookTheBooks.py)](#cookthebookspyscriptscookthebookspy)
- [Authors](#authors)
  - [Acknowledgements](#acknowledgements)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->


## Installing
The last stable analysis base used is **2.3.18**. To install,
```bash
mkdir myRootCore && cd $_
rcSetup Base,2.3.18
git clone https://github.com/UCATLAS/xAODAnaHelpers
cd xAODAnaHelpers && git checkout 00-03-05 && cd ../

git clone https://github.com/UCATLAS/xAODPerfTools

rc find_packages
rc compile
```

### Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)

## Functionality Included

## Authors
- [David Miller](https://github.com/fizisist)
- [Giordon Stark](https://github.com/kratsg)
