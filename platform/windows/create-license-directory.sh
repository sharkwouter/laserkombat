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
  echo "$(pacman -Qi ${1}|grep "^Depends On"|cut -d':' -f 2-|grep -v python|grep -v tzdata|xargs echo)"
}

copy_license_directory ( ) {
	ORIGINAL_DIRECTORY="$(pacman -Ql ${1}| grep '/licenses/.*/$'|cut -f2 -d ' ')"
  if [ -z "${ORIGINAL_DIRECTORY}" ]; then
    echo "No licenses directory found for dependency ${1}. File list:"
    pacman -Ql ${1}
    echo "Please add license manually if required by the type of license."
    return
  fi
  ORIGINAL_DIRECTORY="${ORIGINAL_DIRECTORY::-1}"
  DIRECTORY_NAME="$(echo ${ORIGINAL_DIRECTORY}|rev|cut -f1 -d'/'|rev)"
	if [ -d "${ORIGINAL_DIRECTORY}" ]; then
		if [ ! -d "${LICENSE_DIRECTORY}/${DIRECTORY_NAME}" ]; then
			echo "Adding license for ${1} to ${LICENSE_DIRECTORY}/${DIRECTORY_NAME}"
    else
			echo "Directory for ${1}, ${LICENSE_DIRECTORY}/${DIRECTORY_NAME}, already exists"
      return
		fi
		cp -rf "${ORIGINAL_DIRECTORY}" "${LICENSE_DIRECTORY}/"
		for dependency in $(get_dependencies "${1}"); do
			if [ "${dependency}" == "None" ]; then
				continue
			fi
      echo "Found dependency ${dependency} for ${1}"
			copy_license_directory "${dependency}"
		done
	else
		echo "Failed to find license for library ${1}"
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