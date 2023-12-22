#!/bin/bash
#python archive_artifacts.py --name LamasTinyHUD --src-dir /e/source/LamasTinyHUD/ --dll /e/source/LamasTinyHUD/build/Release/LamasTinyHUD.dll --pdb /e/source/LamasTinyHUD/build/Release/LamasTinyHUD.pdb

python scripts/archive_artifacts.py --name LamasTinyHUD --dll build/Release/LamasTinyHUD.dll --pdb build/Release/LamasTinyHUD.pdb

