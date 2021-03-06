%define rushit_version 0.1

Name:		rushit
Version:	%{rushit_version}
Release:	1%{?dist}
Summary:	scriptable performance tool

Group:		Applications/Internet
License:	ASL 2.0
URL:		https://github.com/jsitnicki/rushit/
Source0:	rushit-%{version}.tar.gz

BuildRequires:	libcmocka-devel

%description
rushit is a tool for performance testing highly configurable
via lua scripts

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
# rhel 'install' command does not support '-D' option properly, we need to
# explicitly create the full paths here
mkdir -p $RPM_BUILD_ROOT/%{_bindir}/    \
    $RPM_BUILD_ROOT/%{_datadir}/rushit/ \
    $RPM_BUILD_ROOT/%{_docdir}/rushit/
install -p -t $RPM_BUILD_ROOT/%{_bindir}/ tcp_stream tcp_rr
install -p -m 0644 -t $RPM_BUILD_ROOT/%{_datadir}/rushit/ scripts/*.lua
install -p -m 0644 -t $RPM_BUILD_ROOT/%{_docdir}/rushit/ README.md README.neper.md

%files
%defattr(-,root,root)
%{_bindir}/*
%{_datadir}/rushit/*.lua
%{_docdir}/rushit/README*
