%define 	req_libvirt_version 0.6.2
%if 0%{?suse_version} || 0%{?sles_version} 
%define		php_confdir %{_sysconfdir}/php5/conf.d
%define		php_extdir 	%{_libdir}/php5/extensions
%else
%define		php_confdir %{_sysconfdir}/php.d 
%define		php_extdir  %{_libdir}/php/modules
%endif

Name:		php-libvirt
Version:	0.3a
Release:	1%{?dist}
Summary:	PHP langueage binding for Libvirt
Group:		Development/Libraries/PHP
License:	PHP
URL:		http://phplibvirt.cybersales.cz/
Source0:	php-libvirt.tar.bz2
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:	php-devel
BuildRequires:	libvirt-devel >= %{req_libvirt_version}
Requires:		libvirt >= %{req_libvirt_version}

%description
PHP language bindings for Libvirt API. 
For more details see: http://phplibvirt.cybersales.cz/ http://www.libvirt.org/ http://www.php.net/

%package -n php-libvirt-doc
Summary:	Document of php-libvirt
Group:		Development/Libraries/PHP
Requires:	php-libvirt = %{version}

%description -n php-libvirt-doc
PHP language bindings for Libvirt API. 
For more details see: http://phplibvirt.cybersales.cz/ http://www.libvirt.org/ http://www.php.net/

This package contain the document for php-libvirt.

%prep
%setup -q -n php-libvirt-%{version}
phpize

%build
%configure
./configure --enable-libvirt --libdir=%{_libdir} --prefix=%{_prefix}
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot} INSTALL_ROOT=%{buildroot}
mkdir -p "%{buildroot}%{_defaultdocdir}/php-libvirt/"
cp -r doc "%{buildroot}%{_defaultdocdir}/php-libvirt/"
mkdir -p "%{buildroot}%{php_confdir}"
echo -e "; Enable libvirt extension module\nextension=libvirt.so" > "%{buildroot}%{php_confdir}/libvirt.ini"
strip "%{buildroot}%{php_extdir}/libvirt.so"

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{php_extdir}/libvirt.so
%config(noreplace) %{php_confdir}/libvirt.ini

%files -n php-libvirt-doc
%defattr(-,root,root,-)
%doc
%{_defaultdocdir}/php-libvirt/

%changelog

*Fri Dec 24 2010 <liyong@skybility.com> 0.3a
-Add a lot of APIs.
