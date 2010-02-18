#!/bin/sh

LOG=job_queue.log.gen

function write_cluster() {
   i=$1

   echo "101 0$i.-1 Job Machine"
   echo "103 0$i.-1 Args \"1m\""
   echo "103 0$i.-1 BufferBlockSize 32768"
   echo "103 0$i.-1 BufferSize 524288"
   echo "103 0$i.-1 ClusterId 1"
   echo "103 0$i.-1 Cmd \"/bin/sleep\""
   echo "103 0$i.-1 CommittedTime 0"
   echo "103 0$i.-1 CompletionDate 0"
   echo "103 0$i.-1 CondorPlatform \"\$CondorPlatform: X86_64-LINUX_RHEL5 \$\""
   echo "103 0$i.-1 CondorVersion \"\$CondorVersion: 7.4.3 Feb 10 2010 BuildID: RH-7.4.3-0.1.el5 PRE-RELEASE \$\""
   echo "103 0$i.-1 CoreSize -1"
   echo "103 0$i.-1 CumulativeSuspensionTime 0"
   echo "103 0$i.-1 CurrentHosts 0"
   echo "103 0$i.-1 DiskUsage 22"
   echo "103 0$i.-1 DiskUsage_RAW 22"
   echo "103 0$i.-1 Environment \"\""
   echo "103 0$i.-1 Err \"/dev/null\""
   echo "103 0$i.-1 ExecutableSize 22"
   echo "103 0$i.-1 ExecutableSize_RAW 22"
   echo "103 0$i.-1 ExitBySignal FALSE"
   echo "103 0$i.-1 ExitStatus 0"
   echo "103 0$i.-1 FileSystemDomain \"filesystem.local\""
   echo "103 0$i.-1 ImageSize 22"
   echo "103 0$i.-1 ImageSize_RAW 22"
   echo "103 0$i.-1 In \"/dev/null\""
   echo "103 0$i.-1 Iwd \"/mnt/pool/gridmonkey\""
   echo "103 0$i.-1 JobLeaseDuration 1200"
   echo "103 0$i.-1 JobNotification 2"
   echo "103 0$i.-1 JobPrio 0"
   echo "103 0$i.-1 JobUniverse 5"
   echo "103 0$i.-1 KillSig \"SIGTERM\""
   echo "103 0$i.-1 LastSuspensionTime 0"
   echo "103 0$i.-1 LeaveJobInQueue FALSE"
   echo "103 0$i.-1 LocalSysCpu 0.000000"
   echo "103 0$i.-1 LocalUserCpu 0.000000"
   echo "103 0$i.-1 MaxHosts 1"
   echo "103 0$i.-1 MinHosts 1"
   echo "103 0$i.-1 NeverCreateJobSandbox TRUE"
   echo "103 0$i.-1 NiceUser FALSE"
   echo "103 0$i.-1 NumCkpts 0"
   echo "103 0$i.-1 NumCkpts_RAW 0"
   echo "103 0$i.-1 NumJobStarts 0"
   echo "103 0$i.-1 NumRestarts 0"
   echo "103 0$i.-1 NumSystemHolds 0"
   echo "103 0$i.-1 OnExitHold FALSE"
   echo "103 0$i.-1 OnExitRemove TRUE"
   echo "103 0$i.-1 Out \"/dev/null\""
   echo "103 0$i.-1 Owner \"gridmonkey\""
   echo "103 0$i.-1 PeriodicHold FALSE"
   echo "103 0$i.-1 PeriodicRelease FALSE"
   echo "103 0$i.-1 PeriodicRemove FALSE"
   echo "103 0$i.-1 QDate $(date +%s)"
   echo "103 0$i.-1 Rank 0.000000"
   echo "103 0$i.-1 RemoteSysCpu 0.000000"
   echo "103 0$i.-1 RemoteUserCpu 0.000000"
   echo "103 0$i.-1 RemoteWallClockTime 0.000000"
   echo "103 0$i.-1 RequestCpus 1"
   echo "103 0$i.-1 RequestDisk DiskUsage"
   echo "103 0$i.-1 RequestMemory ceiling(ifThenElse(JobVMMemory =!= UNDEFINED, JobVMMemory, ImageSize / 1024.000000))"
   echo "103 0$i.-1 Requirements (Arch == \"X86_64\") && (OpSys == \"LINUX\") && (Disk >= DiskUsage) && (((Memory * 1024) >= ImageSize) && ((RequestMemory * 1024) >= ImageSize)) && (TARGET.FileSystemDomain == MY.FileSystemDomain)"
   echo "103 0$i.-1 RootDir \"/\""
   echo "103 0$i.-1 ShouldTransferFiles \"NO\""
   echo "103 0$i.-1 TotalSuspensions 0"
   echo "103 0$i.-1 TransferErr FALSE"
   echo "103 0$i.-1 TransferFiles \"NEVER\""
   echo "103 0$i.-1 TransferIn FALSE"
   echo "103 0$i.-1 TransferOut FALSE"
   echo "103 0$i.-1 User \"gridmonkey@domain.local\""
   echo "103 0$i.-1 UserLog \"/mnt/pool/gridmonkey/test.log\""
   echo "103 0$i.-1 WantCheckpoint FALSE"
   echo "103 0$i.-1 WantRemoteIO TRUE"
   echo "103 0$i.-1 WantRemoteSyscalls FALSE"
}

function write_job() {
   i=$1; j=$2

   echo "103 $i.$j AutoCluster 3"
   echo "103 $i.$j AutoClusterAttrs \"ImageSize,JobUniverse,JobStart,LastPeriodicCheckpoint,LastCheckpointPlatform,NumCkpts,RequestCpus,RequestMemory,RequestDisk,DiskUsage,FileSystemDomain,Requirements,NiceUser,ConcurrencyLimits\""
   echo "103 $i.$j BytesRecvd 0.000000"
   echo "103 $i.$j BytesSent 0.000000"
   echo "103 $i.$j CurrentHosts 0"
   echo "103 $i.$j HoldReasonCode 6"
   echo "103 $i.$j HoldReason \"Error from slot1@node.local: Failed to execute '/bin/sleep' with arguments 1m: No such file or directory\""
   echo "103 $i.$j HoldReasonSubCode 2"
   echo "103 $i.$j JobRunCount 1"
   echo "103 $i.$j LastJobStatus $((RANDOM % 4 + 1))"
   echo "103 $i.$j JobStatus $((RANDOM % 4 + 1))"
   echo "103 $i.$j LastRejMatchReason \"no match found\""
   echo "103 $i.$j LastRemoteHost \"slot1@node.local\""
   echo "103 $i.$j LastSuspensionTime 0"
   echo "103 $i.$j NumJobMatches 2"
   echo "103 $i.$j NumJobStarts 1"
   echo "103 $i.$j NumShadowStarts 1"
   echo "103 $i.$j OrigMaxHosts 1"
   echo "103 $i.$j RemoteWallClockTime 1.000000"
   echo "103 $i.$j StartdPrincipal \"1.6.4.3\""
   echo "103 $i.$j Submission \"ha-schedd@#$i\""
   echo "103 $i.$j WantMatchDiagnostics TRUE"
   echo "103 $i.$j ProcId $j"
   echo "103 $i.$j LastRejMatchTime $(date +%s)"
   echo "103 $i.$j LastPublicClaimId \"<1.6.4.3:54006>#1266255858#595#...\""
   echo "103 $i.$j LastMatchTime $(date +%s)"
   echo "103 $i.$j LastJobLeaseRenewal $(date +%s)"
   echo "103 $i.$j JobStartDate $(date +%s)"
   echo "103 $i.$j JobCurrentStartDate $(date +%s)"
   echo "103 $i.$j GlobalJobId \"ha-schedd@#$i.$j#$(date +%s)\""
   echo "103 $i.$j EnteredCurrentStatus $(date +%s)"
}


do_cluster=0; cluster=0; proc=0; inc_cluster=1;
while [ $cluster -lt 100 ]; do
   if [ $inc_cluster -eq 1 ]; then
      if [ $do_cluster -eq 1 ]; then
         write_cluster $cluster >> $LOG
      fi
      cluster=$((cluster + 1)); proc=0; do_cluster=1
   fi

   if [ $do_cluster -eq 1 -a $((RANDOM % 3)) -eq 1 ]; then
      write_cluster $cluster >> $LOG
      do_cluster=0
   fi

   write_job $cluster $proc >> $LOG

#   sleep 1

   proc=$((proc+1))

   inc_cluster=$((RANDOM % 10))
done
