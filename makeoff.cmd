#
# (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#


rm -frv ./include/libcurl
mkdir -p --verbose ./include/libcurl
	
rm -frv ./include/libssh
mkdir -p --verbose ./include/libssh

rm -fr ./shared
mkdir --verbose ./shared






rm -frv ./_openssl
mkdir --verbose ./_openssl

rm -frv ./_libssh
mkdir -p --verbose ./_libssh

rm -frv ./_libxml2
mkdir -p --verbose ./_libxml2



#BEWARE:MAY_BE_UNSTABLE_THUS_MAY_DAMAGE_ENTIRE_PROCESS::git clone https://github.com/curl/curl.git
wget    --no-check-certificate    https://curl.haxx.se/download/curl-7.50.2.tar.gz
tar xzvf curl-7.50.2.tar.gz
rm -fr ./_libcurl
mv ./curl-7.50.2 ./_libcurl
rm -fr ./curl-7.50.2
rm curl-7.50.2.tar.gz

cd ./_libcurl 
./configure --without-libssh --without-ssl
make
#cp --verbose ./lib/.libs/libcurl.so* ../shared
#BEWARE:TRANSFORM_SOFTLINK_INTO_FILE
cp --verbose ./lib/.libs/libcurl.so ../shared
cp --verbose ./include/curl/curl.h ../include/libcurl
cp --verbose ./include/curl/curlbuild.h ../include/libcurl
cp --verbose ./include/curl/curlrules.h ../include/libcurl
cp --verbose ./include/curl/curlver.h ../include/libcurl
cp --verbose ./include/curl/easy.h ../include/libcurl
cp --verbose ./include/curl/mprintf.h ../include/libcurl
cp --verbose ./include/curl/multi.h ../include/libcurl
cp --verbose ./include/curl/stdcheaders.h ../include/libcurl
cp --verbose ./include/curl/typecheck-gcc.h ../include/libcurl
cd ..


#BEWARE:DONT_DEL_OPENSSL_PRODUCT_UNTILL_DEPLOYED_LATER
wget  https://www.openssl.org/source/old/1.0.0/openssl-1.0.0k.tar.gz
tar xzvf openssl-1.0.0k.tar.gz
rm -fr ./_openssl
mv ./openssl-1.0.0k ./_openssl
rm -fr ./openssl-1.0.0k
rm openssl-1.0.0k.tar.gz

cd ./_openssl
./config --openssldir=/_product
make 
ZT_CURRDIR=$(pwd) && make INSTALL_PREFIX=$ZT_CURRDIR install_sw
cd ..



# Don't del _openssl/_product untill deployed !


git clone git://git.libssh.org/projects/libssh.git _libssh.src
cd _libssh
ZS_CURRDIR=$(pwd) && cmake -DCMAKE_INSTALL_DIR=$ZS_CURRDIR -DCMAKE_BUILD_TYPE=Debug ../_libssh.src \
	-DOPENSSL_CRYPTO_LIBRARY=$ZS_CURRDIR/../_openssl/_product/lib/libcrypto.a \
	-DOPENSSL_INCLUDE_DIR=$ZS_CURRDIR/../_openssl/_product/include \
	-DOPENSSL_SSL_LIBRARY=$ZS_CURRDIR/../_openssl/_product/lib/libssl.a
make
#cp --verbose ./src/libssh.so* ../shared
#BEWARE:TRANSFORM_SOFTLINK_INTO_FILE
cp --verbose ./src/libssh.so ../shared
cp  ../_libssh.src/include/libssh/callbacks.h ../include/libssh
cp  ../_libssh.src/include/libssh/legacy.h ../include/libssh
cp  ../_libssh.src/include/libssh/libssh.h ../include/libssh
cp  ../_libssh.src/include/libssh/sftp.h ../include/libssh
cd ..



git clone https://github.com/GNOME/libxml2.git _libxml2

#BEWARE:YOU_MAY_NEED::apt-get install python-de
#BEWARE:YOU_MAY_NEED::apt-get install autogen
cd _libxml2
./autogen.sh
make

#cp --verbose ./.libs/libxml2.so* ../shared
#BEWARE:TRANSFORM_SOFTLINK_INTO_FILE
cp --verbose ./.libs/libxml2.so ../shared
cp -r --verbose ./include/libxml  ../include/
cd ..



rm -frv ./_openssl
rm -frv ./_libcurl
rm -frv ./_libssh
rm -frv ./_libssh.src
rm -frv ./_libxml2

