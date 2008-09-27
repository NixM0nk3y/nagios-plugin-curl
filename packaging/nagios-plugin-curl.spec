Summary: curl-based web monitoring plugin for Nagios
Name: nagios-plugin-curl
Version: 0.0.1
Release: 0.1
License: GPL
Group: Applications/System
Source: %{name}-%{version}.tar

#TODO: publish
#URL: 

BuildRoot: %{_tmppath}/%{name}-root

# usually too old versions packaged in the distributions!!
#BuildRequires: gengetopt
BuildRequires: curl-devel
Requires: curl

Distribution: rhel5
Packager: Andreas Baumann <abaumann@yahoo.com>


%description
Nagios plugin with similar command line options and functionality
as 'check_http', but based on the cURL web library.

%prep
%setup


%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_libdir}/nagios/plugins/
install -D -m0755 src/check_curl $RPM_BUILD_ROOT/%{_libdir}/nagios/plugins/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%dir /%{_libdir}/nagios/plugins/
/%{_libdir}/nagios/plugins/check_curl

%changelog
* Thu Sep 25 2008 Andreas Baumann <abaumann@yahoo.com> 0.0.1-0.1
- first preliminary packaged release
