#!/bin/bash

# ${PROJECT_ROOT_DIR}/object/MODULE_NAME 에 OBJECT_PATH의 static library를 object 파일로 풀어놓는다.
# extract objects.sh [ARCHIVE_PATH] [EXTRACT_DIR]

ARCHIVE_PATH=$(cd "$(dirname $1)"; pwd)
ARCHIVE_FILE=$(basename $1)
EXTRACT_DIR=$2
SCRIPT_PATH=`pwd -P`

echo "extract_object: $1 $2"

OBJECT_PATH=$EXTRACT_DIR/${ARCHIVE_FILE}

rm -rf $OBJECT_PATH 2> /dev/null
mkdir -p $OBJECT_PATH 2> /dev/null
cd $OBJECT_PATH

ar t $ARCHIVE_PATH/$ARCHIVE_FILE | while read line; do
  if [[ $line == *.o ]]; then
    ar x $ARCHIVE_PATH/$ARCHIVE_FILE $line
  fi
done

