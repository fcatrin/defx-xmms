%define name xmms-defx
%define version 0.9.9
%define release 1

Summary: Multi effects plugin for XMMS.
Name: %{name}
Version: %{version}
Release: %{release}
License: GPL
Group: Applications/Multimedia
URL: http://defx.sourceforge.net/
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root
Vendor: Franco Catrin L.
Requires: xmms >= 1.0.0, glib >= 1.2.7, gtk+ >= 1.2.7
BuildPrereq: xmms-devel

%define	xmmsgeneraldir	%(xmms-config --effect-plugin-dir)

%description
Multi Effects plugin for XMMS.  Use this plugin if you want to
add simple but cool effects to your favorite songs.
Current effects include : karaoke, panning, flanger, chorus, 
phaser and reverberation.

%prep
%setup -q -n %{name}-%{version}

%build
make

%install
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%doc doc COPYING README
%{xmmsgeneraldir}/libdefx.so

%changelog
* Sat Oct 19 2002 Franco Catrin <fcatrin@tuxpan.cl>
- Initial RPM release.

