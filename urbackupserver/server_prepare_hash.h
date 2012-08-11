#ifndef SERVER_PREPARE_HASH_H
#define SERVER_PREPARE_HASH_H

#include "../Interface/Thread.h"
#include "../Interface/File.h"
#include "../Interface/Pipe.h"

#include "ChunkPatcher.h"
#include "../urbackupcommon/sha2/sha2.h"

class INotEnoughSpaceCallback
{
public:
	virtual bool handle_not_enough_space(const std::wstring &path)=0;
};


class BackupServerPrepareHash : public IThread, public IChunkPatcherCallback
{
public:
	BackupServerPrepareHash(IPipe *pPipe, IPipe *pExitpipe, IPipe *pOutput, IPipe *pExitpipe_hash, int pClientid);
	~BackupServerPrepareHash(void);

	void operator()(void);
	
	bool isWorking(void);

	static std::string build_chunk_hashs(IFile *f, IFile *hashoutput, INotEnoughSpaceCallback *cb, bool ret_sha2, IFile *copy);
	static bool writeRepeatFreeSpace(IFile *f, const char *buf, size_t bsize, INotEnoughSpaceCallback *cb);
	static bool writeFileRepeat(IFile *f, const char *buf, size_t bsize);

	void next_chunk_patcher_bytes(const char *buf, size_t bsize);

private:
	std::string hash(IFile *f);
	std::string hash_with_patch(IFile *f, IFile *patch);

	IPipe *pipe;
	IPipe *exitpipe;
	IPipe *output;
	IPipe *exitpipe_hash;

	int clientid;

	sha512_ctx ctx;

	ChunkPatcher chunk_patcher;
	
	volatile bool working;
};

#endif //SERVER_PREPARE_HASH_H