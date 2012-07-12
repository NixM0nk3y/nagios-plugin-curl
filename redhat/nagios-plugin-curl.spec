# Nagios Plugin Curl RPM spec file
#
# Copyright (C) 2012

%define rhel 0
%define rhel5 0
%define rhel6 0
%if 0%{?rhel_version} >= 500 && 0%{?rhel_version} <= 599
%define dist rhel5
%define rhel 1
%define rhel5 1
%endif
%if 0%{?rhel_version} >= 600 && 0%{?rhel_version} <= 699
%define dist rhel6
%define rhel 1
%define rhel6 1
%endif

%define centos 0
%if 0%{?centos_version} >= 500 && 0%{?centos_version} <= 599
%define dist centos5
%define centos 1
%define rhel5 1
%endif

%if 0%{?centos_version} >= 600 && 0%{?centos_version} <= 699
%define dist centos6
%define centos 1
%define rhel6 1
%endif

%define fedora 0
%define fc14 0
%if 0%{?fedora_version} == 14
%define dist fc14
%define fc14 1
%define fedora 1
%endif
%define fc15 0  
%if 0%{?fedora_version} == 15
%define dist fc15
%define fc15 1
%define fedora 1
%endif
%define fc16 0  
%if 0%{?fedora_version} == 16
%define dist fc16
%define fc16 1   
%define fedora 1
%endif
%define fc17 0  
%if 0%{?fedora_version} == 17
%define dist fc17
%define fc17 1   
%define fedora 1
%endif

%define suse 0
%if 0%{?suse_version} == 1140
%define dist osu114
%define suse 1
%endif
%if 0%{?suse_version} > 1140
%define dist osu121
%define suse 1
%endif

%define sles 0
%if 0%{?sles_version} == 11
%define dist sle11
%define sles 1
%endif

Summary: curl-based web monitoring plugin for Nagios
Name: nagios-plugin-curl
Version: 0.0.4
Release: 0.1
License: GPLv3
Group: Applications/System
Source: %{name}_%{version}.tar.gz

URL: https://github.com/andreasbaumann/nagios-plugin-curl

BuildRoot: %{_tmppath}/%{name}-root

# Build dependencies
###

%if %{rhel} || %{centos} || %{fedora}
BuildRequires: pkgconfig
%endif
%if %{suse} || %{sles}
BuildRequires: pkg-config
%endif

%if %{rhel} || %{centos} || %{fedora}
BuildRequires: curl-devel
Requires: curl
%endif

%if %{suse} || %{sles}
BuildRequires: libcurl-devel
Requires: libcurl
%endif

# Check if 'Distribution' is really set by OBS (as mentioned in bacula)
%if ! 0%{?opensuse_bs}
Distribution: %{dist}
%endif

Packager: Andreas Baumann <abaumann@yahoo.com>


%description
Nagios plugin with similar command line options and functionality
as 'check_http', but based on the cURL web library.

%prep
%setup

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_libdir}/nagios/plugins/
install -D -m0755 src/check_curl $RPM_BUILD_ROOT/%{_libdir}/nagios/plugins/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%dir /%{_libdir}/nagios/
%dir /%{_libdir}/nagios/plugins/
/%{_libdir}/nagios/plugins/check_curl

%changelog
* Wed Mar 07 2012 Andreas Baumann <abaumann@yahoo.com> 0.0.4-0.1
- added handling of cookies (for instance for JSESSIONID)
- added POST parameters

* Mon Sep 28 2009 Andreas Baumann <abaumann@yahoo.com> 0.0.3-0.1
- added client certificate and self-signed SSL chain handling

* Wed Dec 17 2008 Andreas Baumann <abaumann@yahoo.com> 0.0.2-0.1
- fixes for 64-bit, upgrade to 0.0.2

* Thu Sep 25 2008 Andreas Baumann <abaumann@yahoo.com> 0.0.1-0.1
- first preliminary packaged release
