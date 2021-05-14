#include "trie.hpp"

int8_t stack[100];
nodeInfo stackSS[4096];
subtreeInfo subtrees[4096];
uint64_t depthVector[4096];

void printString(uint64_t *str, uint64_t length)
{
    printf("string: ");
    for (int i = 0; i < length; i++)
    {
        printf("%ld ", str[i]);
    }
    printf("\n");
}

void printDFUDS(bitmap::Bitmap *dfuds, uint64_t nNodes)
{
    printf("dfuds: ");
    for (int i = 0; i < nNodes; i++)
    {
        printf("%ld, ", getNodeCod(dfuds, i));
    }
    printf("\n");
}

void treeBlock::grow(uint64_t extraNodes)
{
    dfuds->Realloc((maxNodes + extraNodes) * sizeof(uint64_t) * 8);

    maxNodes = maxNodes + extraNodes;
}

void treeBlock::shrink(uint64_t deletedNodes)
{
    dfuds->Realloc((maxNodes - deletedNodes) * sizeof(uint64_t) * 8);
    maxNodes = maxNodes - deletedNodes;    
}

uint64_t getNodeCod(bitmap::Bitmap *dfuds, NODE_TYPE node)
{     
    return dfuds->GetValPos(node * nBranches, nBranches);
}

void setNodeCod(bitmap::Bitmap *dfuds, NODE_TYPE node, uint64_t nodeCod)
{
    // printf("%ld\n", nodeCod);
    dfuds->SetValPos(node * nBranches, nodeCod, nBranches);
}

treeBlock *treeBlock::getPointer(uint64_t curFrontier)
{
    return ((frontierNode *)frontiers)[curFrontier].pointer;
}

uint64_t treeBlock::getPreOrder(uint64_t curFrontier)
{
    return ((frontierNode *)frontiers)[curFrontier].preOrder;
}

void treeBlock::setPreOrder(uint64_t curFrontier, uint64_t preOrder)
{
    ((frontierNode *)frontiers)[curFrontier].preOrder = preOrder;
}

void treeBlock::setPointer(uint64_t curFrontier, treeBlock *pointer)
{
    ((frontierNode *)frontiers)[curFrontier].pointer = pointer;
}

uint64_t symbol2NodeT(uint64_t symbol)
{
    return (uint64_t)1 << (nBranches - symbol - 1);
}

uint64_t getNChildrenT(uint64_t row){

    return __builtin_popcount(row);   
}

uint8_t getChildSkipT(uint64_t row, int col)
{
    return __builtin_popcount(row >> (nBranches - col));  
}

uint8_t getChildT(uint64_t row, int col)
{
    if (((row >> (nBranches - col - 1)) & 1) == 0){
        return -1;
    }
    return __builtin_popcount(row >> (nBranches - col - 1));
}

uint64_t getInsertT(uint64_t row, int col)
{
    int col_digit = row >> (nBranches - col - 1) & 1;

    if ((int) row + (1 << (nBranches - col - 1)) < 0){
        printf("fucked up, row %ld, col: %d\n", row, col);
    }

    if (col_digit == 1)
        return row;
    else
        return row + (1 << (nBranches - col - 1));    
}

treeBlock *createNewTreeBlock(uint64_t rootDepth, uint64_t nNodes, uint64_t maxNodes)
{
    treeBlock *tBlock = (treeBlock *)malloc(sizeof(treeBlock));
    tBlock->dfuds = new bitmap::Bitmap(maxNodes * sizeof(uint64_t) * 8);
    tBlock->rootDepth = rootDepth;
    tBlock->nNodes = nNodes;
    tBlock->frontiers = NULL;
    tBlock->nFrontiers = 0;
    tBlock->maxNodes = maxNodes;
    return tBlock;
}

trieNode *createNewTrieNode()
{
    trieNode *tNode = (trieNode *)malloc(sizeof(trieNode));
    for (int i = 0; i < nBranches; i++)
        tNode->children[i] = NULL;

    tNode->block = NULL;
    return tNode;
}

NODE_TYPE treeBlock::selectSubtree(uint64_t maxDepth, uint64_t &subTreeSize, uint64_t &depthSelectN)
{
    NODE_TYPE node = 0;

    uint64_t depth;

    uint64_t curFrontier = 0;

    //  Corresponds to stackSS, subtrees, depthVector
    uint64_t ssTop = 0, subtreeTop = 0, depthTop = 0;

    uint64_t cNodeCod = getNodeCod(dfuds, 0);

    stackSS[ssTop].preorder = 0;
    stackSS[ssTop++].nChildren = getNChildrenT(cNodeCod);

    node++;

    depth = rootDepth + 1;

    uint64_t nextFrontierPreOrder;

    if (nFrontiers == 0 || curFrontier >= nFrontiers)
        nextFrontierPreOrder = -1;
    else
        nextFrontierPreOrder = getPreOrder(curFrontier);

    for (uint64_t i = 1; i < nNodes; ++i)
    {
        if (i == nextFrontierPreOrder)
        {
            ++curFrontier;
            if (nFrontiers == 0 || curFrontier >= nFrontiers)
                nextFrontierPreOrder = -1;
            else
                nextFrontierPreOrder = getPreOrder(curFrontier);
            --stackSS[ssTop - 1].nChildren;
        }

        else if (depth < maxDepth)
        {
            stackSS[ssTop].preorder = i;
            cNodeCod = getNodeCod(dfuds, i);
            stackSS[ssTop++].nChildren = getNChildrenT(cNodeCod);
            depth++;
        }
        else
            --stackSS[ssTop - 1].nChildren;
        while (ssTop > 0 && stackSS[ssTop - 1].nChildren == 0)
        {
            subtrees[subtreeTop].preorder = stackSS[ssTop - 1].preorder;
            subtrees[subtreeTop++].subtreeSize = i - stackSS[ssTop - 1].preorder + 1;
            --ssTop;
            depthVector[depthTop++] = --depth;
            if (ssTop == 0)
                break;
            else
                stackSS[ssTop - 1].nChildren--;
        }
    }
    uint64_t nodemin, min, posmin;

    uint64_t lowerB = 0.25 * nNodes, upperB = 0.75 * nNodes;
    uint64_t diff;

    nodemin = subtrees[0].preorder,
    min = nNodes - 2 * subtrees[0].subtreeSize;
    posmin = 0;

    for (uint64_t i = 1; i < subtreeTop; ++i)
    {
        diff = nNodes - 2 * subtrees[i].subtreeSize;
        if (diff < 0)
            diff = -diff;
        if (diff < min)
        {
            min = diff;
            nodemin = subtrees[i].preorder;
            posmin = i;
        }
    }
    subTreeSize = subtrees[posmin].subtreeSize;
    depthSelectN = depthVector[posmin];
    return nodemin;
}

void treeBlock::insert(NODE_TYPE node, uint64_t str[], uint64_t length, uint64_t level, uint64_t maxDepth, uint64_t curFrontier)
{
    if (length == 0){
        return;
    }
    // if (str[0] == 64){
        
    //     printf("Wtf\n");
    //     printString(str, length);
    // }
    NODE_TYPE nodeOriginal = node;
    uint16_t maxBlockSize = 256;

    if (frontiers != NULL && curFrontier < nFrontiers && node == getPreOrder(curFrontier))
    {
        uint64_t nodeCod = getNodeCod(dfuds, node);
        setNodeCod(dfuds, node, getInsertT(nodeCod,str[0]));
        getPointer(curFrontier)->insert(0, str, length, level, maxDepth, 0);

        return;
    }
    else if (length == 1)
    {
        uint64_t nodeCod = getNodeCod(dfuds, node);
        setNodeCod(dfuds, node, getInsertT(nodeCod,str[0]));
        return;
    }
    else if (nNodes + length - 1 <= maxNodes)
    {
        node = skipChildrenSubtree(node, str[0], level, maxDepth, curFrontier);
        length--;

        NODE_TYPE destNode = nNodes + length - 1;
        NODE_TYPE origNode = nNodes - 1;

        while (origNode >= node)
        {
            uint64_t origNodeCod = getNodeCod(dfuds, origNode);
            setNodeCod(dfuds, destNode, origNodeCod);
            destNode--;
            origNode--;
        }

        uint64_t nodeCod = getNodeCod(dfuds, nodeOriginal);
        
        setNodeCod(dfuds, nodeOriginal, getInsertT(nodeCod,str[0]));
        origNode++;

        for (uint64_t i = 1; i <= length; i++)
        {
            setNodeCod(dfuds, origNode, symbol2NodeT(str[i]));
            nNodes++;
            origNode++;
        }

        if (frontiers != NULL)
            for (uint64_t i = curFrontier; i < nFrontiers; ++i)
                setPreOrder(i, getPreOrder(i) + length);
    }
    else
    {
        // Restrict maxNodes to maxBlockSize (256)
        if (nNodes + length - 1 <= maxBlockSize)
        {
            grow(length - 1);
            insert(node, str, length, level, maxDepth, curFrontier);
        }
        else
        {
            uint64_t subTreeSize, depthSelectedNode;
            NODE_TYPE selectedNode = selectSubtree(maxDepth, subTreeSize, depthSelectedNode);
            NODE_TYPE origSelectedNode = selectedNode;
        
            uint64_t frontier;
            for (frontier = 0; frontier < nFrontiers; frontier++)
                if (getPreOrder(frontier) > selectedNode)
                    break;

            uint64_t frontierSelectedNode = frontier;
            NODE_TYPE insertionNode = node;
            NODE_TYPE destNode = 0;

            uint64_t copiedNodes = 0;
            bool insertionInNewBlock = false;
            bool isInRoot = false;

            uint64_t newPointerIndex = 0;
            uint64_t copiedFrontier = 0;

            frontierNode *newPointerArray = NULL;
            if (nFrontiers > 0)
            {
                newPointerArray = (frontierNode *)malloc(sizeof(frontierNode) * (nFrontiers + 5));

                if (newPointerArray == nullptr){
                    printf("rip\n");
                }
            }
            
            uint64_t curFrontierNewBlock = 0;
            while (copiedNodes < subTreeSize)
            {
                if (selectedNode == node)
                {
                    insertionInNewBlock = true;
                    if (destNode != 0)
                        insertionNode = destNode;
                    else
                    {
                        insertionNode = node;
                        isInRoot = true;
                    }
                    curFrontierNewBlock = copiedFrontier;
                }

                if (newPointerArray != NULL && frontier < nFrontiers && selectedNode == getPreOrder(frontier))
                {
                    newPointerArray[newPointerIndex].preOrder = destNode;
                    newPointerArray[newPointerIndex].pointer = getPointer(frontier);
                    frontier++;
                    newPointerIndex++;
                    copiedFrontier++;
                }

                setNodeCod(dfuds, destNode, getNodeCod(dfuds, selectedNode));
                if (selectedNode != origSelectedNode)
                {
                    setNodeCod(dfuds, selectedNode, 0);
                }
                selectedNode += 1;
                destNode += 1;
                copiedNodes += 1;
            }

            bool insertionBeforeSelectedTree = true;
            if (!insertionInNewBlock && frontier <= curFrontier)
                insertionBeforeSelectedTree = false;

            treeBlock *new_block = createNewTreeBlock(rootDepth, subTreeSize);

            if (newPointerIndex == 0)
            {
                if (newPointerArray != NULL)
                    free(newPointerArray);
                frontiers = realloc(frontiers, sizeof(frontierNode) * (nFrontiers + 1));
                if (frontiers == nullptr){
                    printf("rip\n");
                }
                for (uint64_t i = nFrontiers; i > frontierSelectedNode; --i)
                {
                    setPointer(i, getPointer(i - 1));
                    setPreOrder(i, getPreOrder(i - 1) - subTreeSize + 1);
                }
                setPreOrder(frontierSelectedNode, origSelectedNode);
                setPointer(frontierSelectedNode, new_block);
                nFrontiers ++;
            }
            else
            {
                newPointerArray = (frontierNode *)realloc(newPointerArray, sizeof(frontierNode) * (newPointerIndex));
                if (newPointerArray == nullptr){
                    printf("rip\n");
                }
                new_block->frontiers = newPointerArray;
                new_block->nFrontiers = newPointerIndex;
                setPreOrder(frontierSelectedNode, origSelectedNode);
                setPointer(frontierSelectedNode, new_block);

                for (uint64_t i = frontierSelectedNode + 1; frontier < nFrontiers; ++i, ++frontier)
                {
                    setPointer(i, getPointer(frontier));
                    setPreOrder(i, getPreOrder(frontier) - subTreeSize + 1);
                }
                nFrontiers = nFrontiers - copiedFrontier + 1;
                frontiers = realloc(frontiers, sizeof(frontierNode) * (nFrontiers));
                if (frontiers == nullptr){
                    printf("rip\n");
                }
            }

            frontier = frontierSelectedNode + 1;
            origSelectedNode++;

            while (selectedNode < nNodes)
            {
                setNodeCod(dfuds, origSelectedNode, getNodeCod(dfuds, selectedNode));
                if (selectedNode == node)
                    insertionNode = origSelectedNode;
                selectedNode++;
                origSelectedNode++;
            }
            if (subTreeSize > length)
                shrink(subTreeSize - 1 - length + 1);
            else
                shrink(subTreeSize - 1);

            nNodes -= (subTreeSize - 1);

            if (!insertionBeforeSelectedTree)
                curFrontier -= copiedFrontier;

            if (insertionInNewBlock)
            {
                if (isInRoot){
                    insert(insertionNode, str, length, level, maxDepth, curFrontier);
                }
                else {
                    new_block->insert(insertionNode, str, length, level, maxDepth, curFrontierNewBlock);
                }
            }
            else
            {
                insert(insertionNode, str, length, level, maxDepth, curFrontier);
            }
        }
    }
}

NODE_TYPE treeBlock::skipChildrenSubtree(NODE_TYPE &node, uint64_t symbol, uint64_t &curLevel, uint64_t maxLevel, uint64_t &curFrontier)
{
    if (curLevel == maxLevel)
        return node;
    uint64_t sTop = -1;

    uint64_t cNodeCod = getNodeCod(dfuds, node);

    uint8_t skipChild = (uint8_t)getChildSkipT(cNodeCod, symbol);

    uint64_t nChildren = getNChildrenT(cNodeCod);

    uint64_t diff = nChildren - skipChild;

    uint64_t stack[100];
    stack[++sTop] = nChildren;

    NODE_TYPE curNode = node + 1;

    if (frontiers != NULL && curFrontier < nFrontiers && curNode > getPreOrder(curFrontier))
        ++curFrontier;

    uint64_t nextFrontierPreOrder;

    if (nFrontiers == 0 || curFrontier >= nFrontiers)
        nextFrontierPreOrder = -1;
    else
        nextFrontierPreOrder = getPreOrder(curFrontier);

    ++curLevel;
    while (curNode < nNodes && sTop >= 0 && diff < stack[0])
    {
        if (curNode == nextFrontierPreOrder)
        {
            ++curFrontier;
            if (nFrontiers == 0 || curFrontier >= nFrontiers)
                nextFrontierPreOrder = -1;
            else
                nextFrontierPreOrder = getPreOrder(curFrontier);
            --stack[sTop];
        }
        // We don't count the leaf level
        else if (curLevel + 1 < maxLevel)
        {
            cNodeCod = getNodeCod(dfuds, curNode);
            stack[++sTop] = getNChildrenT(cNodeCod);
            ++curLevel;
        }
        else
            --stack[sTop];

        ++curNode;
        while (sTop >= 0 && stack[sTop] == 0)
        {
            --sTop;
            --curLevel;
            if (sTop >= 0)
                --stack[sTop];
        }
    }
    return curNode;
}

NODE_TYPE treeBlock::child(treeBlock *&p, NODE_TYPE &node, uint64_t symbol, uint64_t &curLevel, uint64_t maxLevel, uint64_t &curFrontier)
{

    uint64_t cNodeCod = getNodeCod(dfuds, node);
    uint8_t soughtChild = (uint8_t)getChildT(cNodeCod,symbol);
    if (soughtChild == (uint8_t)-1)
    {
        return NULL_NODE;
    }
    if (curLevel == maxLevel && soughtChild != (uint64_t)-1)
        return node;

    NODE_TYPE curNode;

    if (frontiers != NULL && curFrontier < nFrontiers && node == getPreOrder(curFrontier))
    {
        p = getPointer(curFrontier);
        curFrontier = 0;
        NODE_TYPE auxNode = 0;
        curNode = p->skipChildrenSubtree(auxNode, symbol, curLevel, maxLevel, curFrontier);
    }
    else
        curNode = skipChildrenSubtree(node, symbol, curLevel, maxLevel, curFrontier);

    return curNode;
}

void insertar(treeBlock *root, uint64_t *str, uint64_t length, uint64_t level, uint64_t maxDepth)
{
    treeBlock *curBlock = root;
    uint64_t i = 0;

    NODE_TYPE curNode = 0;
    uint64_t curFrontier = 0;
    walkTree(curBlock, str, length, maxDepth, curNode, i, level, curFrontier); 
    curBlock->insert(curNode, &str[i], length - i, level, maxDepth, curFrontier);
}


bool walkTree(treeBlock *curBlock, uint64_t str[], int strlen, int maxDepth, NODE_TYPE &curNode, uint64_t &i, uint64_t &level, uint64_t &curFrontier){

    NODE_TYPE curNodeAux = 0;
    
    while (i < strlen)
    {
        curNodeAux = curBlock->child(curBlock, curNode, str[i], level, maxDepth, curFrontier);

        if (curNodeAux == (NODE_TYPE)-1)
            return false;

        curNode = curNodeAux;

        if (curBlock->nFrontiers > 0 && curFrontier < curBlock->nFrontiers && curNode == curBlock->getPreOrder(curFrontier))
        {
            curBlock = curBlock->getPointer(curFrontier);
            curNode = (NODE_TYPE)0;
        }
        i++;
    }
    return true;        
}

treeBlock *walkTrie(trieNode *tNode, uint64_t *str, uint64_t &i){

    while (tNode->children[str[i]])
        tNode = tNode->children[str[i++]];
    while (i < L1)
    {
        tNode->children[str[i]] = createNewTrieNode();
        tNode = tNode->children[str[i]];
        i++;
    }
    treeBlock *tBlock = NULL;
    if (tNode->block == NULL)
    {
        tBlock = createNewTreeBlock();
        tNode->block = tBlock;
    }
    else
        tBlock = (treeBlock *)tNode->block;
    return tBlock;    
}

void insertTrie(trieNode *tNode, uint64_t *str, uint64_t length, uint64_t maxDepth)
{
    uint64_t i = 0;
    
    treeBlock *tBlock = walkTrie(tNode, str, i);
    
    insertar(tBlock, &str[i], length - i, i, maxDepth);
}

uint64_t *proc_str(char *line, int &strLen, int &maxDepth){
    char *pos;
    int extra_char = 1;
    if ((pos = strchr(line, '\n')) != NULL)
        *pos = '\0';
    else
        extra_char = 0;
    strLen = (int)strlen(line) - extra_char;

    if (strLen > maxDepth){
        maxDepth = strLen;
    }
    uint64_t *str = (uint64_t *)malloc(sizeof(uint64_t) * strLen);
    
    for (int j = 0; j < strLen; j++)
    {
        str[j] = ((int)line[j]) % nBranches;
        if (str[j] == 64){
            printf("wtffff\n");
        }
    }
    return str;
}