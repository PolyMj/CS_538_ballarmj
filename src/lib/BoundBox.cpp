#include "BoundBox.hpp" 
 
namespace potato { 
    // Convert bounding box to integer box 
    BoundBoxi convertBoundBox(BoundBoxf &boxf) { 
        BoundBoxi boxi; 
        boxi.start = floorV(boxf.start); 
        boxi.end = floorV(boxf.end); 
        return boxi; 
    }; 

    BoundBoxi convertBoundBox(BoundBoxd &boxd) { 
        BoundBoxi boxi; 
        boxi.start = floorV(boxd.start); 
        boxi.end = floorV(boxd.end); 
        return boxi; 
    }; 
}; 
