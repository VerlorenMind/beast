#ifndef BEAST_BEASTDECODER_H
#define BEAST_BEASTDECODER_H

#include <fstream>
#include <cstdint>
#include <set>
#include <queue>

#include "TrellisDecoder.h"

enum InsertionStatus
{
    INSERTED,
    REPLACED,
    DISCARDED,
    ENDED,
};
class BeastDecoder : public TrellisDecoder {
protected:
    double min_metric = -1;
    uint64_t min_candidate = 0;
    Node *fwdTree, *fwdTreeBuffer, *bkwTree, *bkwTreeBuffer;
    unsigned fwdTreeSize, fwdTreeBufferSize, bkwTreeSize, bkwTreeBufferSize;
    InsertionStatus insertNode(const Node& node);
    void init();
public:
    BeastDecoder(unsigned int n, unsigned int k, std::ifstream& filename);
    BeastDecoder(unsigned int n, unsigned int k, int **h);
    double decode(double* x, int* u, double delta);
    ~BeastDecoder();
};

#endif //BEAST_BEASTDECODER_H
