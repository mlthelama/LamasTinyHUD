import argparse
import os
import zipfile
from pathlib import Path


def make_rel_archive(a_args):
    languages: list[str] = ["english", "czech", "french", "german", "italian", "japanese", "polish", "russian",
                            "spanish"]
    resource_extensions: list[str] = ["*.ttf", "*.otf", "*.svg"]

    archive: zipfile = zipfile.ZipFile(a_args.name + ".zip", "w", zipfile.ZIP_DEFLATED)

    os.chdir('../')
    v_pwd: str = os.getcwd()
    if a_args.dir:
        v_pwd: str = os.path.join(os.getcwd(), a_args.dir)

    archive.write(os.path.join(v_pwd, a_args.dll), "SKSE/Plugins/{}".format(os.path.basename(a_args.dll)))
    archive.write(os.path.join(v_pwd, a_args.pdb), "SKSE/Plugins/{}".format(os.path.basename(a_args.pdb)))
    archive.write(os.path.join(v_pwd, "LamasTinyHUD.ini"), "SKSE/Plugins/LamasTinyHUD.ini")

    v_path: str = os.path.join(v_pwd, "extern", "resources", "resources")
    for extension in resource_extensions:
        for path in Path(v_path).rglob(extension):
            archive.write(path, os.path.join("SKSE/Plugins/resources", path.relative_to(v_path)))

    archive.write(os.path.join(v_pwd, "mcm", "LamasTinyHUD.esl"), "LamasTinyHUD.esl")
    v_path: str = os.path.join(v_pwd, "mcm", "scripts")
    for path in Path(v_path).glob('*.pex'):
        archive.write(path, os.path.join("scripts/", path.name))

    archive.write(os.path.join(v_pwd, "mcm", "Config", "LamasTinyHUD", "config.json"),
                  "mcm/Config/LamasTinyHUD/config.json")
    archive.write(os.path.join(v_pwd, "mcm", "Config", "LamasTinyHUD", "settings.ini"),
                  "mcm/Config/LamasTinyHUD/settings.ini")
    for lang in languages:
        archive.write(os.path.join(v_pwd, "mcm", "Interface", "Translations", "LamasTinyHUD_english.txt"),
                      "Interface/Translations/LamasTinyHUD_" + lang + ".txt")


def parse_arguments():
    parser = argparse.ArgumentParser(description="archive build artifacts for distribution")
    parser.add_argument("--dll", type=str, help="the full dll path", required=True)
    parser.add_argument("--name", type=str, help="the project name", required=True)
    parser.add_argument("--pdb", type=str, help="the full pdb path", required=True)
    parser.add_argument("--dir", type=str, help="additonal", required=False)
    return parser.parse_args()


def main():
    out = "artifacts"
    try:
        os.mkdir(out)
    except FileExistsError:
        pass
    os.chdir(out)

    args = parse_arguments()
    make_rel_archive(args)


if __name__ == "__main__":
    main()
