import argparse
import os
import zipfile
from pathlib import Path


def make_rel_archive(a_args):
    languages: list[str] = ["english", "czech", "french", "german", "italian", "japanese", "polish", "russian", "spanish"]

    archive: zipfile = zipfile.ZipFile(a_args.name + ".zip", "w", zipfile.ZIP_DEFLATED)
    archive.write(a_args.dll, "SKSE/Plugins/{}".format(os.path.basename(a_args.dll)))
    archive.write(a_args.pdb, "SKSE/Plugins/{}".format(os.path.basename(a_args.pdb)))
    archive.write(os.path.join(a_args.src_dir, "LamasTinyHUD.ini"), "SKSE/Plugins/LamasTinyHUD.ini")

    v_path: str = os.path.join(a_args.src_dir, "resources")
    for path in Path(v_path).rglob('*.png'):
        archive.write(path, os.path.join("SKSE/Plugins/resources/", path.parent.name, path.name))

    archive.write(os.path.join(a_args.src_dir, "mcm", "LamasTinyHUD.esl"), "LamasTinyHUD.esl")
    archive.write(os.path.join(a_args.src_dir, "mcm", "scripts", "LamasTinyHUD_MCM.pex"),
                  "scripts/LamasTinyHUD_MCM.pex")
    archive.write(os.path.join(a_args.src_dir, "mcm", "Config", "LamasTinyHUD", "config.json"),
                  "mcm/Config/LamasTinyHUD/config.json")
    archive.write(os.path.join(a_args.src_dir, "mcm", "Config", "LamasTinyHUD", "settings.ini"),
                  "mcm/Config/LamasTinyHUD/settings.ini")
    for lang in languages:
        archive.write(os.path.join(a_args.src_dir, "mcm", "Interface", "Translations", "LamasTinyHUD_english.txt"),
                      "Interface/Translations/LamasTinyHUD_" + lang + ".txt")


def parse_arguments():
    parser = argparse.ArgumentParser(description="archive build artifacts for distribution")
    parser.add_argument("--dll", type=str, help="the full dll path", required=True)
    parser.add_argument("--name", type=str, help="the project name", required=True)
    parser.add_argument("--pdb", type=str, help="the full pdb path", required=True)
    parser.add_argument("--src-dir", type=str, help="the project root source directory", required=True)
    return parser.parse_args()


def main():
    out: str = "artifacts"
    try:
        os.mkdir(out)
    except FileExistsError:
        pass
    os.chdir(out)

    args = parse_arguments()
    make_rel_archive(args)


if __name__ == "__main__":
    main()
