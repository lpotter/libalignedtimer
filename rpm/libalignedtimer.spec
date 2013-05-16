# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.26
# 

Name:       libalignedtimer

# >> macros
# << macros

Summary:    A library of Qt bindings for libalignedtimer
Version:    0.1
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        https://github.com/nemomobile/libalignedtimer
Source0:    %{name}-%{version}.tar.bz2
Source100:  libalignedtimer.yaml
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(libiphb)

%description
This package contains Qt bindings for ofono cellular service
interfaces.


%package declarative
Summary:    Declarative plugin for libalignedtimer
Group:      Development/Tools
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description declarative
This package contains declarative plugin for libalignedtimer.

%package tests
Summary:    unit test for the alignedtimer
Group:      Development/Tools
Requires:   %{name} = %{version}-%{release}

%description tests
This package contains qml test for alignedtimer

%package devel
Summary:    Development files for libalignedtimer
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description devel
This package contains the development header files for libalignedtimer

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export QT_SELECT=5
# << build pre

%qmake5 

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export QT_SELECT=5
# << install pre
%qmake_install

# >> install post
# << install post

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{name}.so.*
# >> files
# << files

%files declarative
%defattr(-,root,root,-)
%{_libdir}/qt5/imports/org/nemomobile/alignedtimer/*
# >> files declarative
# << files declarative

%files tests
%defattr(-,root,root,-)
%{_libdir}/%{name}/tests/tst_qalignedtimer
# >> files tests
# << files tests

%files devel
%defattr(-,root,root,-)
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/libalignedtimer.pc
%{_includedir}/alignedtimer/*.h
# >> files devel
# << files devel
