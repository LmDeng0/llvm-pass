#ifndef OPGRAPH_H___
#define OPGRAPH_H___

#include <vector>
#include <string>
#include <iostream>
#include <memory>

typedef enum
{
    OPGRAPH_OP_NOP = 0,
    OPGRAPH_OP_SEXT,
    OPGRAPH_OP_ZEXT,
    OPGRAPH_OP_TRUNC,
    OPGRAPH_OP_INPUT,
    OPGRAPH_OP_OUTPUT,
    OPGRAPH_OP_PHI,
    OPGRAPH_OP_CONST,
    OPGRAPH_OP_ADD,
    OPGRAPH_OP_SUB,
    OPGRAPH_OP_MUL,
    OPGRAPH_OP_DIV,
    OPGRAPH_OP_AND,
    OPGRAPH_OP_OR,
    OPGRAPH_OP_XOR,
    OPGRAPH_OP_SHL,
    OPGRAPH_OP_SHRA,
    OPGRAPH_OP_SHRL,
    OPGRAPH_OP_LOAD,
    OPGRAPH_OP_STORE,
    OPGRAPH_OP_GEP,
    OPGRAPH_OP_ICMP,
    OPGRAPH_OP_SHR,
} OpGraphOpCode;

std::ostream& operator <<(std::ostream &os, const OpGraphOpCode &opcode);
std::istream& operator >>(std::istream &is, OpGraphOpCode &opcode);

#include <CGRA/MRRG.h>

class MRRG;
class MRRGNode;

class OpGraphNode
{
    public:
        OpGraphNode(std::string name)
        {
            this->name = name;
        };
        virtual ~OpGraphNode();

        std::string name;
};

class OpGraphVal;

class OpGraphOp : public OpGraphNode
{
    public:
        OpGraphOp(std::string name);
        OpGraphOp(std::string name, OpGraphOpCode opcode);
        ~OpGraphOp();

        OpGraphOpCode opcode;

        bool setOperand(int op_num, OpGraphVal* val);

        std::vector<OpGraphVal*> input;
        OpGraphVal* output;

        // parser functions
        void parserUpdate(std::map<std::string, std::string>);

        // scheduler stuff
        int    asap_cycle;
        int    alap_cycle;

        int    scheduled_cycle;

        int    op_latency;

    private:
        // translation map for text opcodes to enum
        static std::map<std::string, OpGraphOpCode> opcode_map;

        friend std::ostream& operator<<(std::ostream& output, const OpGraphOp& op);
        friend std::ostream& operator <<(std::ostream &os, const OpGraphOpCode &opcode);
        friend std::istream& operator >>(std::istream &is, OpGraphOpCode &opcode);
};

class OpGraphVal : public OpGraphNode
{
    public:
        OpGraphVal(std::string name);
        OpGraphVal(std::string name, OpGraphOp* input_op);
        ~OpGraphVal();

        OpGraphOp* input;

        // all fanouts
        std::vector<OpGraphOp*>         output;

        // scheduler stuff
        std::vector<unsigned int>       output_operand;     // this is the operand port that this value feeds to at the downstream Op. Parallel array to the output vector
        std::vector<unsigned int>       output_delay;       // this is the critical timing delay of the mapped path in pico seconds - ie IF there are multiple registers in the path, this is the longest path
        std::vector<unsigned int>       output_latency;       // this is the critical timing delay of the mapped path in pico seconds - ie IF there are multiple registers in the path, this is the longest path

        // Fanout mapping results
        std::vector<std::vector<MRRGNode*>> fanout_result;

        friend std::ostream& operator<<(std::ostream& output, const OpGraphVal& val);
};

// Multiple output DFG with val/net nodes between op nodes.
class OpGraph
{
    public:
        OpGraph();              // creates an empty OpGraph
        ~OpGraph();

        //float getCost(float pfactor);
        int getMaxCycle();

        void print_dot(std::ostream & s = std::cout);
        void printDOTwithOps(std::ostream &s = std::cout);

        void debug_check();

        // scheduling
        bool computeASAP(unsigned int* l);
        bool computeALAP(unsigned int l);
        bool scheduleASAP(unsigned int * latency = NULL);

        bool verifySchedule();


        std::vector<OpGraphOp*> inputs;
        std::vector<OpGraphOp*> outputs;

        // list of all nodes in the graph
        std::vector<OpGraphOp*>     op_nodes;
        std::vector<OpGraphVal*>    val_nodes;

//    private:
//        std::shared_ptr<MRRG> mappedMRRG;
};

#endif

