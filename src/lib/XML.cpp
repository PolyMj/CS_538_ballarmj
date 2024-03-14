#include "XML.hpp" 
 
namespace potato { 
    void findElementWithTag(XMLElement *node, string tag, vector<XMLElement*> &foundNodes) { 
        if(!strcmp(node->Name(), tag.c_str())) { 
            foundNodes.push_back(node); 
        } 
 
        if(node->FirstChildElement() != nullptr) { 
            // For each child 
            for(XMLElement *child = node->FirstChildElement(); 
                child != nullptr; 
                child = child->NextSiblingElement()) { 
                findElementWithTag(child, tag, foundNodes); 
            } 
        } 
    }; 
 
    void findSVGElements(XMLDocument &doc, string itemTag, vector<XMLElement*> &desiredNodes) { 
        vector<XMLElement*> svgNodes; 
        findElementWithTag(doc.FirstChildElement(), "svg", svgNodes); 
        XMLElement *svgNode = svgNodes.at(0); 
        findElementWithTag(svgNode, itemTag, desiredNodes); 
    }; 
}; 
