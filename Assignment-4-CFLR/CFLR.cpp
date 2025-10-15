/**
 * CFLR.cpp
 * @author kisslune 
 */

#include "A4Header.h"

using namespace SVF;
using namespace llvm;
using namespace std;

int main(int argc, char **argv)
{
    auto moduleNameVec =
            OptionBase::parseOptions(argc, argv, "Whole Program Points-to Analysis",
                                     "[options] <input-bitcode...>");

    LLVMModuleSet::buildSVFModule(moduleNameVec);

    SVFIRBuilder builder;
    auto pag = builder.build();
    pag->dump();

    CFLR solver;
    solver.buildGraph(pag);
    // TODO: complete this method
    solver.solve();
    solver.dumpResult();

    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}


void CFLR::solve()
{
    //  1. implement the grammar production rules into code;
    //  2. implement the dynamic-programming CFL-reachability algorithm.
    while (!workList.empty())
    {
        CFLREdge edge = workList.pop();
        
        // Rule: PT -> Addr
        if (edge.label == Addr)
        {
            if (!graph->hasEdge(edge.src, edge.dst, PT))
            {
                graph->addEdge(edge.src, edge.dst, PT);
                workList.push(CFLREdge(edge.src, edge.dst, PT));
            }
        }

        // Process forward and backward rules
        processForwardRules(edge);
        processBackwardRules(edge);
    }
}

void CFLR::processForwardRules(const CFLREdge& edge)
{
    unsigned u = edge.src;
    unsigned v = edge.dst;
    EdgeLabel label = edge.label;

    auto &succMap = graph->getSuccessorMap();
    if (succMap.count(v))
    {
        for (auto const&[succ_label, succ_nodes] : succMap.at(v))
        {
            for (unsigned w : succ_nodes)
            {
                // Rule: PT -> PT Copy
                if (label == PT && succ_label == Copy)
                {
                    if (!graph->hasEdge(u, w, PT))
                    {
                        graph->addEdge(u, w, PT);
                        workList.push(CFLREdge(u, w, PT));
                    }
                }
                // Rule: SV -> PT Store
                else if (label == PT && succ_label == Store)
                {
                    if (!graph->hasEdge(u, w, SV))
                    {
                        graph->addEdge(u, w, SV);
                        workList.push(CFLREdge(u, w, SV));
                    }
                }
                // Rule: PV -> PT Load
                else if (label == PT && succ_label == Load)
                {
                    if (!graph->hasEdge(u, w, PV))
                    {
                        graph->addEdge(u, w, PV);
                        workList.push(CFLREdge(u, w, PV));
                    }
                }
            }
        }
    }
}

void CFLR::processBackwardRules(const CFLREdge& edge)
{
    unsigned u = edge.src;
    unsigned v = edge.dst;
    EdgeLabel label = edge.label;

    auto &predMap = graph->getPredecessorMap();
    if (predMap.count(u))
    {
        for (auto const&[pred_label, pred_nodes] : predMap.at(u))
        {
            for (unsigned w : pred_nodes)
            {
                // Rule: PT -> PT Copy (backward)
                if (pred_label == PT && label == Copy)
                {
                    if (!graph->hasEdge(w, v, PT))
                    {
                        graph->addEdge(w, v, PT);
                        workList.push(CFLREdge(w, v, PT));
                    }
                }
                // Rule: PT -> SV PT
                else if (pred_label == SV && label == PT)
                {
                    if (!graph->hasEdge(w, v, PT))
                    {
                        graph->addEdge(w, v, PT);
                        workList.push(CFLREdge(w, v, PT));
                    }
                }
                // Rule: PT -> PV PT
                else if (pred_label == PV && label == PT)
                {
                    if (!graph->hasEdge(w, v, PT))
                    {
                        graph->addEdge(w, v, PT);
                        workList.push(CFLREdge(w, v, PT));
                    }
                }
            }
        }
    }
}
