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
    solver.solve();
    solver.dumpResult();

    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}

void CFLR::solve()
{
    // Helper function: add edge if not present
    auto addEdgeIfAbsent = [&](unsigned src, unsigned dst, EdgeLabel label) {
        if (!graph->hasEdge(src, dst, label))
        {
            graph->addEdge(src, dst, label);
            workList.push(CFLREdge(src, dst, label));
        }
    };
    
    // Step 1: Collect all initial edges and nodes
    std::unordered_set<unsigned> nodes;
    std::vector<CFLREdge> initialEdges;
    
    auto &succMap = graph->getSuccessorMap();
    for (auto &[src, labelMap] : succMap)
    {
        nodes.insert(src);
        for (auto &[label, dstSet] : labelMap)
        {
            for (unsigned dst : dstSet)
            {
                nodes.insert(dst);
                initialEdges.push_back(CFLREdge(src, dst, label));
            }
        }
    }
    
    // Step 2: Push initial edges to worklist
    for (const auto &edge : initialEdges)
    {
        workList.push(edge);
    }
    
    // Step 3: Add epsilon edges
    for (unsigned node : nodes)
    {
        addEdgeIfAbsent(node, node, EdgeLabelType::VF);
        addEdgeIfAbsent(node, node, EdgeLabelType::VFBar);
        addEdgeIfAbsent(node, node, EdgeLabelType::VA);
        addEdgeIfAbsent(node, node, EdgeLabelType::VABar);
    }
    
    // Step 4: Define grammar rules
    // Unary rules
    std::unordered_map<EdgeLabel, std::vector<EdgeLabel>> unaryRules{
        {EdgeLabelType::Copy,    {EdgeLabelType::VF}},
        {EdgeLabelType::CopyBar, {EdgeLabelType::VFBar}},
    };
    
    // Binary rules: result ::= left right
    std::vector<std::tuple<EdgeLabel, EdgeLabel, EdgeLabel>> binaryRules = {
        // PT rules
        {EdgeLabelType::PT,     EdgeLabelType::VFBar,   EdgeLabelType::AddrBar},
        {EdgeLabelType::PTBar,  EdgeLabelType::Addr,    EdgeLabelType::VF},
        
        // VF rules
        {EdgeLabelType::VF,     EdgeLabelType::VF,      EdgeLabelType::VF},
        {EdgeLabelType::VF,     EdgeLabelType::SV,      EdgeLabelType::Load},
        {EdgeLabelType::VF,     EdgeLabelType::PV,      EdgeLabelType::Load},
        {EdgeLabelType::VF,     EdgeLabelType::Store,   EdgeLabelType::VP},
        
        // VFBar rules
        {EdgeLabelType::VFBar,  EdgeLabelType::VFBar,   EdgeLabelType::VFBar},
        {EdgeLabelType::VFBar,  EdgeLabelType::LoadBar, EdgeLabelType::SVBar},
        {EdgeLabelType::VFBar,  EdgeLabelType::LoadBar, EdgeLabelType::VP},
        {EdgeLabelType::VFBar,  EdgeLabelType::PV,      EdgeLabelType::StoreBar},
        
        // VA rules
        {EdgeLabelType::VA,     EdgeLabelType::LV,      EdgeLabelType::Load},
        {EdgeLabelType::VA,     EdgeLabelType::VFBar,   EdgeLabelType::VA},
        {EdgeLabelType::VA,     EdgeLabelType::VA,      EdgeLabelType::VF},
        {EdgeLabelType::VA,     EdgeLabelType::PT,      EdgeLabelType::PTBar},
        
        // VABar rules
        {EdgeLabelType::VABar,  EdgeLabelType::VABar,   EdgeLabelType::VABar},
        {EdgeLabelType::VABar,  EdgeLabelType::VF,      EdgeLabelType::VABar},
        {EdgeLabelType::VABar,  EdgeLabelType::VA,      EdgeLabelType::VFBar},
        {EdgeLabelType::VABar,  EdgeLabelType::PT,      EdgeLabelType::PTBar},
        
        // SV and SVBar rules
        {EdgeLabelType::SV,     EdgeLabelType::Store,   EdgeLabelType::VA},
        {EdgeLabelType::SVBar,  EdgeLabelType::VA,      EdgeLabelType::StoreBar},
        
        // PV and VP rules
        {EdgeLabelType::PV,     EdgeLabelType::PTBar,   EdgeLabelType::VA},
        {EdgeLabelType::VP,     EdgeLabelType::VA,      EdgeLabelType::PT},
        
        // PVBar and VPBar rules
        {EdgeLabelType::PVBar,  EdgeLabelType::VABar,   EdgeLabelType::PT},
        {EdgeLabelType::VPBar,  EdgeLabelType::PTBar,   EdgeLabelType::VABar},
        
        // LV rules
        {EdgeLabelType::LV,     EdgeLabelType::LoadBar, EdgeLabelType::VA},
        
        // LVBar rules
        {EdgeLabelType::LVBar,  EdgeLabelType::VA,      EdgeLabelType::Load},
    };
    
    // Build lookup tables
    std::unordered_map<EdgeLabel, std::vector<std::pair<EdgeLabel, EdgeLabel>>> leftRules;
    std::unordered_map<EdgeLabel, std::vector<std::pair<EdgeLabel, EdgeLabel>>> rightRules;
    
    for (const auto &[result, left, right] : binaryRules)
    {
        leftRules[left].emplace_back(result, right);
        rightRules[right].emplace_back(result, left);
    }
    
    // Step 5: Main loop - CFL-Reachability algorithm
    while (!workList.empty())
    {
        CFLREdge edge = workList.pop();
        
        // Apply unary rules
        if (auto it = unaryRules.find(edge.label); it != unaryRules.end())
        {
            for (EdgeLabel result : it->second)
            {
                addEdgeIfAbsent(edge.src, edge.dst, result);
            }
        }
        
        // Apply binary rules (current edge as left operand)
        if (auto it = leftRules.find(edge.label); it != leftRules.end())
        {
            auto &succMap = graph->getSuccessorMap();
            if (succMap.find(edge.dst) != succMap.end())
            {
                for (const auto &[result, rightLabel] : it->second)
                {
                    if (succMap[edge.dst].find(rightLabel) != succMap[edge.dst].end())
                    {
                        for (unsigned next : succMap[edge.dst][rightLabel])
                        {
                            addEdgeIfAbsent(edge.src, next, result);
                        }
                    }
                }
            }
        }
        
        // Apply binary rules (current edge as right operand)
        if (auto it = rightRules.find(edge.label); it != rightRules.end())
        {
            auto &predMap = graph->getPredecessorMap();
            if (predMap.find(edge.src) != predMap.end())
            {
                for (const auto &[result, leftLabel] : it->second)
                {
                    if (predMap[edge.src].find(leftLabel) != predMap[edge.src].end())
                    {
                        for (unsigned prev : predMap[edge.src][leftLabel])
                        {
                            addEdgeIfAbsent(prev, edge.dst, result);
                        }
                    }
                }
            }
        }
    }
}

void CFLR::processForwardRules(const CFLREdge& edge) {}
void CFLR::processBackwardRules(const CFLREdge& edge) {}
