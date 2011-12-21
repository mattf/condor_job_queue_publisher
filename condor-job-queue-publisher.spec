
Name: condor-job-queue-publisher
Version: 0.43
Release: 1
Summary: Publish a Condor schedd job queue to an AMQP queue

Group: System Environment/Daemons
License: Apache 2.0
URL: https://fedorahosted.org/grid/wiki/CondorJobQueuePublisher
Source0: %{name}-%{version}.tar.gz

BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires: boost-devel qpid-cpp-client-devel log4cpp-devel

%description
%{summary}

%prep
%setup -q

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=%{buildroot}

# Remove test programs
rm $RPM_BUILD_ROOT/%{_bindir}/memory_performance
rm $RPM_BUILD_ROOT/%{_bindir}/sink
rm $RPM_BUILD_ROOT/%{_bindir}/test_Job

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/condor_job_queue_publisher

%changelog
* Mon Dec 19 2011 Brian Bockelman <bbockelm@cse.unl.edu> - 0.43-1
- Initial packaging of job queue publisher.


