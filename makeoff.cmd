rm -frv ./_openssl
mkdir --verbose ./_openssl

rm -frv ./include/libcurl
mkdir -p --verbose ./include/libcurl
	
rm -frv ./include/libssh
mkdir -p --verbose ./include/libssh

rm -fr ./shared
mkdir --verbose ./shared

rm -fr ./_openssh
mkdir --verbose ./_openssh

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
cp --verbose ./lib/.libs/libcurl.so* ../shared
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



wget  https://www.openssl.org/source/old/1.0.0/openssl-1.0.0k.tar.gz
tar xzvf openssl-1.0.0k.tar.gz
rm -fr ./_openssl
mv ./openssl-1.0.0k ./_openssl
rm -fr ./openssl-1.0.0k
#rm openssl-1.0.0k.tar.gz

cd ./_openssl
./config --openssldir=/_product
ZT_CURRDIR=$(pwd) && make INSTALL_PREFIX=$ZT_CURRDIR install
cd ..


#rm -fr  ./_openssh
rm -frv ./_libcurl
