/*-------------------------------------------------------------------------
 *
 * procarray.h
 *	  POSTGRES process array definitions.
 *
 *
 * Portions Copyright (c) 1996-2013, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/storage/procarray.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PROCARRAY_H
#define PROCARRAY_H

#include "storage/standby.h"
#include "utils/snapshot.h"

#include "cdb/cdbpublic.h"

struct DtxContextInfo;         /* cdb/cdbdtxcontextinfo.h */
struct SnapshotData;           /* utils/tqual.h */

extern Size ProcArrayShmemSize(void);
extern void CreateSharedProcArray(void);
extern void ProcArrayAdd(PGPROC *proc);
extern void ProcArrayRemove(PGPROC *proc, TransactionId latestXid);
extern bool ProcArrayEndTransaction(PGPROC *proc, TransactionId latestXid, bool isCommit);
extern void ProcArrayEndGxact(void);
extern void ProcArrayClearTransaction(PGPROC *proc, bool commit);
extern void ClearTransactionFromPgProc_UnderLock(PGPROC *proc, bool commit);

extern void ProcArrayApplyRecoveryInfo(RunningTransactions running);
extern void ProcArrayApplyXidAssignment(TransactionId topxid,
							int nsubxids, TransactionId *subxids);

extern void RecordKnownAssignedTransactionIds(TransactionId xid);
extern void ExpireTreeKnownAssignedTransactionIds(TransactionId xid,
									  int nsubxids, TransactionId *subxids,
									  TransactionId max_xid);
extern void ExpireAllKnownAssignedTransactionIds(void);
extern void ExpireOldKnownAssignedTransactionIds(TransactionId xid);

extern int	GetMaxSnapshotXidCount(void);
extern int	GetMaxSnapshotSubxidCount(void);

extern Snapshot GetSnapshotData(Snapshot snapshot);

extern bool ProcArrayInstallImportedXmin(TransactionId xmin,
							 TransactionId sourcexid);

extern RunningTransactions GetRunningTransactionData(void);

extern bool TransactionIdIsInProgress(TransactionId xid);
extern bool TransactionIdIsActive(TransactionId xid);
extern TransactionId GetOldestXmin(bool allDbs, bool ignoreVacuum);
extern TransactionId GetLocalOldestXmin(bool allDbs, bool ignoreVacuum);
extern TransactionId GetOldestActiveTransactionId(void);

extern VirtualTransactionId *GetVirtualXIDsDelayingChkpt(int *nvxids);
extern bool HaveVirtualXIDsDelayingChkpt(VirtualTransactionId *vxids, int nvxids);

extern PGPROC *BackendPidGetProc(int pid);
extern int	BackendXidGetPid(TransactionId xid);
extern bool IsBackendPid(int pid);

extern VirtualTransactionId *GetCurrentVirtualXIDs(TransactionId limitXmin,
					  bool excludeXmin0, bool allDbs, int excludeVacuum,
					  int *nvxids);
extern VirtualTransactionId *GetConflictingVirtualXIDs(TransactionId limitXmin, Oid dbOid);
extern pid_t CancelVirtualTransaction(VirtualTransactionId vxid, ProcSignalReason sigmode);

extern bool MinimumActiveBackends(int min);
extern int	CountDBBackends(Oid databaseid);
extern void CancelDBBackends(Oid databaseid, ProcSignalReason sigmode, bool conflictPending);
extern int	CountUserBackends(Oid roleid);
extern bool CountOtherDBBackends(Oid databaseId,
					 int *nbackends, int *nprepared);
extern bool HasSerializableBackends(bool allDbs);

extern void XidCacheRemoveRunningXids(TransactionId xid,
						  int nxids, const TransactionId *xids,
						  TransactionId latestXid);
						  
extern PGPROC *FindProcByGpSessionId(long gp_session_id);
extern void UpdateSerializableCommandId(CommandId curcid);

extern void updateSharedLocalSnapshot(struct DtxContextInfo *dtxContextInfo, struct SnapshotData *snapshot, char* debugCaller);

extern void GetSlotTableDebugInfo(void **snapshotArray, int *maxSlots);

extern void getDtxCheckPointInfo(char **result, int *result_size);

extern List *ListAllGxid(void);
extern int GetPidByGxid(DistributedTransactionId gxid);

#endif   /* PROCARRAY_H */
