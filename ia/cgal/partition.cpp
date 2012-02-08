#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <boost/next_prior.hpp>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include <iostream>

#include <CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
#include <boost/shared_ptr.hpp>


struct FaceInfo2
{
    FaceInfo2(){}
    int nesting_level;
    int index;

    bool in_domain(){ 
	return nesting_level%2 == 1;
    }
};


typedef CGAL::Exact_predicates_inexact_constructions_kernel		K;
typedef CGAL::Triangulation_vertex_base_2<K>					Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,K>	Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<K,Fbb>		Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>				TDS;
typedef CGAL::Exact_predicates_tag								Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag>	CDT;
typedef CDT::Point												Point;
typedef CGAL::Polygon_2<K>										Polygon;
typedef CGAL::Polygon_with_holes_2<K>							Polygon_with_holes;
typedef std::list<Polygon_with_holes>							Pwh_list_2;


typedef K::FT							FT;
typedef CGAL::Straight_skeleton_2<K>	Ss;
typedef boost::shared_ptr<Polygon_with_holes> PolygonWithHolesPtr;
typedef boost::shared_ptr<Ss>			SsPtr ;
typedef std::vector<PolygonWithHolesPtr> PolygonWithHolesPtrVector;





void 
mark_domains(CDT& ct, 
             CDT::Face_handle start, 
             int index, 
             std::list<CDT::Edge>& border )
{
	if(start->info().nesting_level != -1){
		return;
	}
	std::list<CDT::Face_handle> queue;
	queue.push_back(start);

	while(! queue.empty()){
		CDT::Face_handle fh = queue.front();
		queue.pop_front();
		if(fh->info().nesting_level == -1){
			fh->info().nesting_level = index;
			for(int i = 0; i < 3; i++){
				CDT::Edge e(fh,i);
				CDT::Face_handle n = fh->neighbor(i);
				if(n->info().nesting_level == -1){
					if(ct.is_constrained(e)) border.push_back(e);
					else queue.push_back(n);
				}
			}
		}
	}
}

//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident 
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void
mark_domains(CDT& cdt)
{
	for(CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
		it->info().nesting_level = -1;
	}

	int index = 0;
	std::list<CDT::Edge> border;
	mark_domains(cdt, cdt.infinite_face(), index++, border);
	while(! border.empty()){
		CDT::Edge e = border.front();
		border.pop_front();
		CDT::Face_handle n = e.first->neighbor(e.second);
		if(n->info().nesting_level == -1){
			mark_domains(cdt, n, e.first->info().nesting_level+1, border);
		}
	}
}

void insert_polygon(CDT& cdt,const Polygon& polygon){
	if ( polygon.is_empty() ) return;
	CDT::Vertex_handle v_prev=cdt.insert(*boost::prior(polygon.vertices_end()));
	for (Polygon::Vertex_iterator vit=polygon.vertices_begin();
								vit!=polygon.vertices_end();++vit)
	{
		CDT::Vertex_handle vh=cdt.insert(*vit);
		cdt.insert_constraint(vh,v_prev);
		v_prev=vh;
	}  
}


Polygon& ask_polygon(Polygon &poly)
{
    int n;
    Point point;
	
    std::cin >> n;
	
    for (int i=0; i<n; ++i)
    {
	std::cin >> point;
	poly.push_back(point);
    }
	
    return poly;
}

int main()
{
	FT lOffset;
    //CGAL::set_ascii_mode(std::cin);
    //CGAL::set_ascii_mode(std::cout);
    CDT cdt;
    Pwh_list_2 difference;

    std::cin >> lOffset;
	
    Polygon polygon;
    ask_polygon(polygon);
	
    int nb_trous = 0;
    std::cin >> nb_trous;
	
    Polygon trou;
    ask_polygon(trou);
    CGAL::difference(polygon, trou, std::back_inserter(difference));

    for (int i=1; i<nb_trous; ++i)
    {
		Pwh_list_2 temp;
		Polygon trou;
		ask_polygon(trou);
		for (Pwh_list_2::const_iterator pwhit=difference.begin(); pwhit != difference.end(); ++pwhit)
		{
			CGAL::difference((*pwhit), trou, std::back_inserter(temp));
		}
		difference = temp;
    }
	
	Pwh_list_2 temp;
	difference.swap(temp);
    for (Pwh_list_2::const_iterator pwhit=temp.begin(); pwhit != temp.end(); ++pwhit)
    {
		PolygonWithHolesPtrVector offset_poly_with_holes = CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(lOffset,*pwhit);
	
		for(PolygonWithHolesPtrVector::const_iterator p=offset_poly_with_holes.begin(); p!=offset_poly_with_holes.end(); ++p)
		{
			difference.push_back(**p);
		}
	}
	
    for (Pwh_list_2::const_iterator pwhit=difference.begin(); pwhit != difference.end(); ++pwhit)
    {
		if (! pwhit->is_unbounded() )
		{
			insert_polygon(cdt, pwhit->outer_boundary());
			for (Polygon_with_holes::Hole_const_iterator hit = pwhit->holes_begin();
			hit != pwhit->holes_end(); ++hit)
			{
			insert_polygon(cdt,(*hit));
			}
		}
		else
			std::cout << "erreur" << std::endl;
    }
	
	
  
    //Mark facets that are inside the domain bounded by the polygon
    mark_domains(cdt);

    int count=0;
    for (CDT::Finite_faces_iterator fit=cdt.finite_faces_begin();
                                  fit!=cdt.finite_faces_end();++fit)
    {
	if ( fit->info().in_domain() )
	{
	    std::cout << "<" << cdt.triangle(fit) << ">";
	    std::cout << &(*fit) << " ";
	    for (int i=0; i<3; ++i)
		std::cout << &(*(fit->neighbor(i))) << " ";
	    std::cout << std::endl;
	    ++count;
	}
    }
    
    std::cout << "There are " << count << " facets in the domain." << std::endl;

    return 0;
}
