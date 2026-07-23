#!/bin/bash

LICENSE_DIRECTORY="third-party-licenses"
EXIT_CODE=0
LIBRARIES=($@)
SCRIPT_NAME="$(basename "$0")"

usage ( ) {
	cat <<EOF
${SCRIPT_NAME} [OPTION] [LIBRARY]...
The ${SCRIPT_NAME} tool makes it easier to comply with licenses of used libraries.
It adds the licenses to the ${LICENSE_DIRECTORY} directory.

options:
  -h		  Print this message
  -l		  List installed libraries

example:
  If your project has included SDL2, SDL2_ttf and jsoncpp:

    ${SCRIPT_NAME} sdl2 sdl2-ttf jsoncpp
    
  To get a list of installed libraries to get the name right use:

    ${SCRIPT_NAME} -l

EOF
}

get_dependencies ( ) {
  echo "$(pacman -Qi ${1}|grep "^Depends On"|cut -d':' -f 2-|xargs echo)"
}

copy_license_directory ( ) {
	if [ -d "/mingw64/share/licenses/${1}" ]; then
		if [ ! -d "${LICENSE_DIRECTORY}/${1}" ]; then
			echo "Adding license for ${1} to ${LICENSE_DIRECTORY}"
		fi
		cp -rf "/mingw64/share/licenses//${1}" "${LICENSE_DIRECTORY}/"
		for dependency in $(get_dependencies "${1}"); do
			if [ "${dependency}" == "None" ]; then
				continue
			fi
			if [ ! -d "${LICENSE_DIRECTORY}/${dependency}" ]; then
				echo "Found dependency ${dependency} for ${1}"
			fi
			copy_license_directory "${dependency}"
		done
	else
		echo "Failed to find license for library ${1}"
		EXIT_CODE=2
	fi
}

while getopts "hl" OPTION; do
	case ${OPTION} in
	h)
		usage
		exit 0
	;;
	l)
		psp-pacman -Qq
		exit 0
	;;
	*)
		echo "${OPTION} - Unrecongnized option"
		usage
		exit 1
	;;
	esac
done

mkdir -p "${LICENSE_DIRECTORY}"
for library in ${LIBRARIES[@]}; do
	copy_license_directory "${library}"
done
exit ${EXIT_CODE}