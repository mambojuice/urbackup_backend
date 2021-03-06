class CClientThread;
class IFile;
struct SChunk;

#include "../Interface/Thread.h"
#include "../Interface/Types.h"
#include "../md5.h"

class ChunkSendThread : public IThread
{
public:
	ChunkSendThread(CClientThread *parent);
	~ChunkSendThread(void);

	void operator()(void);

	bool sendChunk(SChunk *chunk);

private:

	CClientThread *parent;
	IFile *file;
	_i64 curr_hash_size;
	_i64 curr_file_size;

	char *chunk_buf;

	bool has_error;

	MD5 md5_hash;
};