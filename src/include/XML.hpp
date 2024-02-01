#pragma once

#include <cmath>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <regex>
#include "Line.hpp"
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;

namespace potato {    
    void findElementWithTag(XMLElement *node, string tag, vector<XMLElement*> &foundNodes);
    void findSVGElements(XMLDocument &doc, string itemTag, vector<XMLElement*> &desiredNodes);
    
    template<typename T, typename C>
    Line<T,C> parseSVGLine(XMLElement *element) {
        Line<T,C> line;
        auto x1 = static_cast<T>(stof(element->Attribute("x1")));
        auto y1 = static_cast<T>(stof(element->Attribute("y1")));
        auto x2 = static_cast<T>(stof(element->Attribute("x2")));
        auto y2 = static_cast<T>(stof(element->Attribute("y2")));        
        line.start = Vec3<T>(x1, y1, static_cast<T>(0));
        line.end = Vec3<T>(x2, y2, static_cast<T>(0));

        string style = element->Attribute("style");
        smatch colorMatch;
        if(regex_search(style, colorMatch, regex("stroke:rgb\\((\\d+),(\\d+),(\\d+)\\)"))) {
            auto r = static_cast<C>(stof(colorMatch[1]));
            auto g = static_cast<C>(stof(colorMatch[2]));
            auto b = static_cast<C>(stof(colorMatch[3]));
            line.color = Vec3<C>(r,g,b);
        }
        else {
            line.color = Vec3<C>(255,255,255);
        }

        return line;
    };
    
    template<typename T, typename C>
    void parseAllSVGLines(XMLDocument &doc, vector<Line<T,C>> &allLines) {        
        vector<XMLElement*> lineNodes;
        findSVGElements(doc, "line", lineNodes);     
        for(int i = 0; i < lineNodes.size(); i++) {            
            allLines.push_back(parseSVGLine<T,C>(lineNodes.at(i)));
        }            
    };  
};
