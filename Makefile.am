ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects


bin_PROGRAMS = upnp-render
upnp_render_SOURCES=src/ConManager.cpp src/Log.cpp src/RControl.cpp src/Xml.cpp src/Config.cpp src/Device.cpp src/Player.cpp src/Service.cpp src/Transport.cpp src/WebPage.cpp src/WebServer.cpp
upnp_render_CPPFLAGS=-std=c++11 -Iinclude

DEBDEP=bash (>= 2.05a-11), awk, procps (>= 1:2.0.7-8), sed (>= 3.02-8), grep (>= 2.4.2-3), coreutils (>= 5.0-5), libupnp1.6.18 (>=1.6.18)

if DEBUG
AM_CFLAGS = -g -DDEBUG -Wall
AM_CXXFLAGS = -g -DDEBUG -Wall
MAKEINIT=0
else
AM_CFLAGS = -march=corei7 -O2
AM_CXXFLAGS = -march=corei7 -O2
MAKEINIT=1
endif

debian_BUILD = $(bin_PROGRAMS)-$(VERSION)

dist_doc_DATA=README

sysconf_DATA= \
config.xml \
web/upnp/128x128.png \
web/upnp/64x64.png \
web/upnp/renderconnmgrSCPD.xml \
web/upnp/rendercontrolSCPD.xml \
web/upnp/rendertransportSCPD.xml \
web/description.xml

install_f =\
echo -n "install to $1"; \
$(MKDIR_P) $(1)/usr/share/$(bin_PROGRAMS); \
$(INSTALL) $(bin_PROGRAMS) $(1)/usr/bin; \
for p in $(sysconf_DATA); do \
$(INSTALL_DATA) $$p "$(1)/usr/share/$(bin_PROGRAMS)/$$p" || exit $$?; \
done; \
echo "Done"

install: install-binPROGRAMS
	@DOLLAR_SIGN@(call install_f,$(prefix))

deb: $(bin_PROGRAMS)
	rm -rf $(debian_BUILD)
	$(MKDIR_P) $(debian_BUILD)/DEBIAN
	echo "Package: $(bin_PROGRAMS)">  $(debian_BUILD)/DEBIAN/control
	echo "Version: $(VERSION)" >> $(debian_BUILD)/DEBIAN/control
	echo "Section: base" >> $(debian_BUILD)/DEBIAN/control
	echo "Priority: optional" >> $(debian_BUILD)/DEBIAN/control
	echo "Architecture: all" >> $(debian_BUILD)/DEBIAN/control
	echo "Depends: $(DEBDEP)" >> $(debian_BUILD)/DEBIAN/control
	echo "Maintainer: Alex D <wvdial@gmail.com>" >> $(debian_BUILD)/DEBIAN/control
	echo "Description: upnp media render" >> $(debian_BUILD)/DEBIAN/control
	@DOLLAR_SIGN@(call install_f,$(debian_BUILD))
	for p in `find $(debian_BUILD) -type f -print | grep -v DEBIAN`; do \
	md5sum $$p | $(SED) 's/$(debian_BUILD)\///'>>$(debian_BUILD)/DEBIAN/md5sum; \
	done
	dpkg-deb --build $(debian_BUILD)
	rm -rf $(debian_BUILD)

