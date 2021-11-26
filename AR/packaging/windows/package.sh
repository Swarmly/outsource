#----------------------------
# Declare variables
project_dir="$1"
package_root_dir="$2"
windows_package_dir="$package_root_dir/windows"
working_dir="$package_root_dir/windows/build-installer"
bin_dir="$3"
mingw_dir="$4"
app_file_name="gcs.exe"
app_dir_name="hermes"
nsis_name="hermes"
app_package_name="$5"
download_tool="wget --tries=42"
hmtool_name="hmtool"

#----------------------------
# Remove previously created artifacts
rm -rf $windows_package_dir

#----------------------------
# Prepare package for deploy

# Creat working directory
mkdir -p $working_dir/$app_dir_name

# Change directory to $working_dir
cd $working_dir

# Copy executable file
cp "$bin_dir/$app_file_name" $app_dir_name
# Copy all built plugins
cp "$bin_dir"/*.dll $app_dir_name
# Remove test related plugins
rm -f $app_dir_name/*test*.dll

# Copy hmtool
cp "$bin_dir"/$hmtool_name $app_dir_name

# Run windeployqt.
# It takes an .exe file or a directory that contains an .exe file as an argument,
# and scans the executable for dependencies.
# Identified dependencies are then copied to the executable's directory.
windeployqt --opengl --compiler-runtime --serialport --network --printsupport --verbose 2 "$app_dir_name/$app_file_name"

cp "$mingw_dir"/libgcc_s_*-1.dll    $app_dir_name
cp "$mingw_dir"/libstdc++-6.dll     $app_dir_name
cp "$mingw_dir"/libwinpthread-1.dll $app_dir_name

# FFMPEG
cp "$mingw_dir"/av*.dll $app_dir_name
cp "$mingw_dir"/swscale*.dll $app_dir_name
cp "$mingw_dir"/swresample*.dll $app_dir_name
cp "$mingw_dir"/libcelt*.dll $app_dir_name
cp "$mingw_dir"/libgnutls*.dll $app_dir_name
cp "$mingw_dir"/libaom*.dll $app_dir_name
cp "$mingw_dir"/libass*.dll $app_dir_name
cp "$mingw_dir"/libcaca*.dll $app_dir_name
cp "$mingw_dir"/libbluray*.dll $app_dir_name
cp "$mingw_dir"/libdav1d*.dll $app_dir_name
cp "$mingw_dir"/libfreetype*.dll $app_dir_name
cp "$mingw_dir"/libgsm*.dll $app_dir_name
cp "$mingw_dir"/libmfx*.dll $app_dir_name
cp "$mingw_dir"/libmodplug*.dll $app_dir_name
cp "$mingw_dir"/libmp3lame*.dll $app_dir_name
cp "$mingw_dir"/libopencore*.dll $app_dir_name
cp "$mingw_dir"/libopenjp2*.dll $app_dir_name
cp "$mingw_dir"/libopenmj*.dll $app_dir_name
cp "$mingw_dir"/libidn2*.dll $app_dir_name
cp "$mingw_dir"/libopus*.dll $app_dir_name
cp "$mingw_dir"/librtmp*.dll $app_dir_name
cp "$mingw_dir"/libsrt*.dll $app_dir_name
cp "$mingw_dir"/libspeex*.dll $app_dir_name
cp "$mingw_dir"/libsoxr*.dll $app_dir_name
cp "$mingw_dir"/libtheora*.dll $app_dir_name
cp "$mingw_dir"/libvorbis*.dll $app_dir_name
cp "$mingw_dir"/libx264*.dll $app_dir_name
cp "$mingw_dir"/libhdr10plus*.dll $app_dir_name
cp "$mingw_dir"/libx265*.dll $app_dir_name
cp "$mingw_dir"/xvidcore*.dll $app_dir_name
cp "$mingw_dir"/libvpx*.dll $app_dir_name
cp "$mingw_dir"/libwebp*.dll $app_dir_name
cp "$mingw_dir"/libxml2*.dll $app_dir_name
cp "$mingw_dir"/libopenal*.dll $app_dir_name
cp "$mingw_dir"/libwavpack*.dll $app_dir_name
cp "$mingw_dir"/postproc*.dll $app_dir_name
cp "$mingw_dir"/libvulkan*.dll $app_dir_name
cp "$mingw_dir"/libbz2*.dll $app_dir_name
cp "$mingw_dir"/libexif*.dll $app_dir_name
cp "$mingw_dir"/libexiv2*.dll $app_dir_name
# TODO: copy frei0r when it's required
cp "$mingw_dir"/libfontconfig*.dll $app_dir_name

# Common
cp "$mingw_dir"/zlib*.dll $app_dir_name

# OpenSSL
cp "$mingw_dir"/libcrypto*.dll $app_dir_name
cp "$mingw_dir"/libssl*.dll $app_dir_name

# Misc: dependencies of other libraries that ffmpeg depends on and opencv deps also
cp "$mingw_dir"/libportaudio*.dll $app_dir_name
cp "$mingw_dir"/libharfbuzz*.dll $app_dir_name
cp "$mingw_dir"/libiconv*.dll $app_dir_name
cp "$mingw_dir"/libicuin*.dll $app_dir_name
cp "$mingw_dir"/libicuuc*.dll $app_dir_name
cp "$mingw_dir"/libicudt*.dll $app_dir_name
cp "$mingw_dir"/libpcre*.dll $app_dir_name
cp "$mingw_dir"/libdouble-conversion*.dll $app_dir_name
cp "$mingw_dir"/libzstd*.dll $app_dir_name
cp "$mingw_dir"/liblzma*.dll $app_dir_name
cp "$mingw_dir"/SDL2*.dll $app_dir_name
cp "$mingw_dir"/libglib*.dll $app_dir_name
cp "$mingw_dir"/libgraphite2*.dll $app_dir_name
cp "$mingw_dir"/libgmp*.dll $app_dir_name
cp "$mingw_dir"/libhogweed*.dll $app_dir_name
cp "$mingw_dir"/libnettle*.dll $app_dir_name
cp "$mingw_dir"/libintl*.dll $app_dir_name
cp "$mingw_dir"/libp11-kit*.dll $app_dir_name
cp "$mingw_dir"/libtasn1*.dll $app_dir_name
cp "$mingw_dir"/libunistring*.dll $app_dir_name
cp "$mingw_dir"/libssp*.dll $app_dir_name
cp "$mingw_dir"/libogg*.dll $app_dir_name
cp "$mingw_dir"/libfribidi*.dll $app_dir_name
cp "$mingw_dir"/libexpat*.dll $app_dir_name
cp "$mingw_dir"/libgomp*.dll $app_dir_name
cp "$mingw_dir"/libffi*.dll $app_dir_name
cp "$mingw_dir"/libbrotli*.dll $app_dir_name
cp "$mingw_dir"/libopencv_core*.dll $app_dir_name
cp "$mingw_dir"/libopencv_highgui*.dll $app_dir_name
cp "$mingw_dir"/libopencv_imgproc*.dll $app_dir_name
cp "$mingw_dir"/libopencv_objdetect*.dll $app_dir_name
cp "$mingw_dir"/libopencv_videoio*.dll $app_dir_name
cp "$mingw_dir"/libopencv_imgcodecs*.dll $app_dir_name
cp "$mingw_dir"/libopencv_calib3d*.dll $app_dir_name
cp "$mingw_dir"/tbb*.dll $app_dir_name
cp "$mingw_dir"/libopenblas*.dll $app_dir_name
cp "$mingw_dir"/libIlmImf*.dll $app_dir_name
cp "$mingw_dir"/libjpeg*.dll $app_dir_name
cp "$mingw_dir"/libtiff*.dll $app_dir_name
cp "$mingw_dir"/libpng*.dll $app_dir_name
cp "$mingw_dir"/libopencv_feature*.dll $app_dir_name
cp "$mingw_dir"/libopencv_flann*.dll $app_dir_name
cp "$mingw_dir"/libHalf*.dll $app_dir_name
cp "$mingw_dir"/libIex*.dll $app_dir_name
cp "$mingw_dir"/libIlmThread*.dll $app_dir_name
cp "$mingw_dir"/libImath*.dll $app_dir_name
cp "$mingw_dir"/libdeflate*.dll $app_dir_name
cp "$mingw_dir"/libgfortran*.dll $app_dir_name
cp "$mingw_dir"/libquadmath*.dll $app_dir_name

#----------------------------
# Build Packages

# Create Installer Executable
$download_tool https://download.microsoft.com/download/3/2/2/3224B87F-CFA0-4E70-BDA3-3DE650EFEBA5/vcredist_x64.exe
cp "$project_dir/packaging/windows/nsis/"* .

# TODO: add license file when it's available
# cp $project_dir/LICENSE .

# Use unicode version of ShellExecAsUser plugin
$download_tool https://nsis.sourceforge.io/mediawiki/images/6/68/ShellExecAsUser_amd64-Unicode.7z
7z x ShellExecAsUser_amd64-Unicode.7z Plugins
makensis -V4 -DX64 "-XOutFile $app_package_name.exe" -X"'!AddPluginDir' $working_dir/Plugins/amd64-unicode" $nsis_name.nsi

# Create Portable ZIP
echo -n > $app_dir_name/portable
7z a $app_package_name.zip $app_dir_name

#----------------------------
# Finalize

# copy to output directory
cp $app_package_name.zip $windows_package_dir
cp $app_package_name.exe $windows_package_dir

#echo ""
#echo "Remove installer build directory: $working_dir"
#rm -rf $working_dir
echo ""
echo "Installer: $windows_package_dir/$app_package_name.exe"
echo "Portable: $windows_package_dir/$app_package_name.zip"