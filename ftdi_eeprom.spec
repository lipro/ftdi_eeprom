Summary:   Tool for creating/reading/erasing/flashing FTDI USB chip eeproms
Name:      ftdi_eeprom
Version:   0.2
Release:   1
Copyright: GPL
Group:     Development/Tools
Vendor:    Intra2net AG
Source:    %{name}-%{version}.tar.gz
Buildroot: /tmp/%{name}-%{version}-root
Requires:  libftdi, libconfuse
BuildRequires: libftdi-devel, libconfuse
Prefix:    /usr

%description
Tool for creating/reading/erasing/flashing FTDI USB chip eeproms 

%prep
%setup -q

%build
autoconf
./configure --prefix=%{prefix}
make

%install
make DESTDIR=$RPM_BUILD_ROOT install

mkdir -p $RPM_BUILD_ROOT/usr/share/doc/%{name}-%{version}
cp ftdi_eeprom/example.conf $RPM_BUILD_ROOT/usr/share/doc/%{name}-%{version}

%clean
rm -fr $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{prefix}/bin/ftdi_eeprom
/usr/share/doc/%{name}-%{version}/*
