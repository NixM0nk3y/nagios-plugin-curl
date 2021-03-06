#!/bin/sh

PACKAGE_NAME=nagios-plugin-curl
PACKAGE_VERSION=0.0.4
OSC_HOME=$HOME/home:andreas_baumann/$PACKAGE_NAME

rm -f ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
make dist-gz
cp ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz $OSC_HOME/${PACKAGE_NAME}_${PACKAGE_VERSION}.tar.gz
cp redhat/$PACKAGE_NAME.spec $OSC_HOME/$PACKAGE_NAME.spec

SIZE=`stat -c '%s' $OSC_HOME/${PACKAGE_NAME}_${PACKAGE_VERSION}.tar.gz`
CHKSUM=`md5sum $OSC_HOME/${PACKAGE_NAME}_${PACKAGE_VERSION}.tar.gz  | cut -f 1 -d' '`

cat tools/$PACKAGE_NAME.dsc > $OSC_HOME/$PACKAGE_NAME.dsc
echo " $CHKSUM $SIZE ${PACKAGE_NAME}_${PACKAGE_VERSION}.tar.gz" >> $OSC_HOME/$PACKAGE_NAME.dsc
