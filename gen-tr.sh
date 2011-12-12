#!/bin/sh
# gen-tr.sh for nomnom.

update()
{
  qmake -project -o gen-tr.pro &&
  lupdate -no-obsolete gen-tr.pro 2>/dev/null &&
  rm -f gen-tr.pro
  exit $?
}

new()
{
  lupdate src/ -ts $1 2>/dev/null && update
}

release()
{
  lrelease -compress $1
  exit $?
}

help()
{
  echo "Usage: $0 [-h] [-n|-u] [.ts file]
 -h  Show this help and exit
 -n  Create a new .ts file
 -u  Update .ts file
Examples:
 $0 -n tr/NomNom_fr_FR.ts ;# create new .ts (for French)
 $0 -u                    ;# update current translations
 $0 -r tr/NomNom_fr_FR.ts ;# create .qm from NomNom_fr_FR.ts"
  exit 0
}

n_flag=0 ; u_flag=0 ; r_flag=0
while [ $# -gt 0 ]
do
  case "$1" in
    -h) help;;
    -n) n_flag=1;;
    -u) u_flag=1;;
    -r) r_flag=1;;
     *) break;;
  esac
  shift
done

[ $u_flag -gt 0 ] && update
[ $# -lt 1 ] && help
[ $n_flag -gt 0 ] && new $1
[ $r_flag -gt 0 ] && release $1
help
