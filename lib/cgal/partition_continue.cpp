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

//#define DEBUG
#ifdef DEBUG
	#define log(s) std::cerr << s << std::endl
#else
	#define log(s)
#endif

#define log_err(s) std::cerr << __FILE__ << '(' << __LINE__ << ") : " << s << std::endl;


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
void mark_domains(CDT& cdt)
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

void insert_polygon(CDT& cdt, const Polygon& polygon){
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

void insert_polygon_with_holes(CDT& cdt, const Polygon_with_holes& polygon)
{
	insert_polygon(cdt, polygon.outer_boundary());
	for (Polygon_with_holes::Hole_const_iterator hit = polygon.holes_begin();
	hit != polygon.holes_end(); ++hit)
	{
		insert_polygon(cdt,(*hit));
	}
}


Polygon& ask_polygon(Polygon &poly)
{
	log("ask_polygon");
	
    int n=0;
    Point point;
	
    log("n ?");
    std::cin >> n;
	
    for (int i=0; i<n; ++i)
    {
		log("point ?");
		std::cin >> point;
		poly.push_back(point);
    }
	
	log("end ask_polygon");
    return poly;
}

/**
 * stdin :
 * 		<nb_trou>
 * 		<description poly1>
 * 		<description poly2>
 * 		...
 *
 * @return false si (nombre de trous < 0)
 */
bool trouer_polygon(const Polygon_with_holes & poly, Pwh_list_2 & result)
{
	log("trouer_polygon");
    Polygon trou;
    int nb_trous = 0;

    log("nb_trous ?");
    std::cin >> nb_trous;

    if (nb_trous < 0) {
		log_err("nombre de trous < 0");
		return false;
	}
    if (nb_trous == 0) return true;
    
    ask_polygon(trou);
    CGAL::difference(poly, trou, std::back_inserter(result));

    
    for (int i=1; i<nb_trous; ++i)
    {
		Pwh_list_2 temp;
		Polygon trou;
		ask_polygon(trou);
		for (Pwh_list_2::const_iterator pwhit=result.begin(); pwhit != result.end(); ++pwhit)
		{
			CGAL::difference((*pwhit), trou, std::back_inserter(temp));
		}
		result = temp;
    }

	log("end trouer_polygon");
    return true;
}

/**
 * stdin :
 *      <offset>
 * 		<description polygon>
 * 		<description trouer polygon>
 */
void ask_map(Polygon_with_holes& map)
{
	log("ask_map");
	
	FT lOffset;
    Pwh_list_2 difference;
    Polygon container;
    
    std::cin >> lOffset;

    ask_polygon(container);
    
    trouer_polygon(Polygon_with_holes(container), difference);

    if (difference.size() != 1)
    {
		log_err("la définition de la map n'est pas cohérente (Err:1)");
		exit(0);
	}
	
	PolygonWithHolesPtrVector offset_poly_with_holes = CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(lOffset,(*difference.begin()));

	if (offset_poly_with_holes.size() != 1)
	{
		log_err("la définition de la map n'est pas cohérente (Err:2)");
		exit(0);
	}

	map = (*offset_poly_with_holes[0]);

	log("end ask_map");
}

int count_facets(CDT & cdt)
{
    int count=0;
    
    for (CDT::Finite_faces_iterator fit=cdt.finite_faces_begin();
                                  fit!=cdt.finite_faces_end();++fit)
    {
		if ( fit->info().in_domain() )
		{
			++count;
		}
    }

    return count;
}

void print_facets(CDT & cdt)
{
    for (CDT::Finite_faces_iterator fit=cdt.finite_faces_begin();
                                  fit!=cdt.finite_faces_end();++fit)
    {
		if ( fit->info().in_domain() )
		{
			// id
			std::cout << "id = " << &(*fit) << ";";
			// points
			std::cout << "points = (";
			std::cout << "(" << fit->vertex(0)->point()[0] << "," << fit->vertex(0)->point()[1] << "),";
			std::cout << "(" << fit->vertex(1)->point()[0] << "," << fit->vertex(1)->point()[1] << "),";
			std::cout << "(" << fit->vertex(2)->point()[0] << "," << fit->vertex(2)->point()[1] << ")";
			std::cout << ");";
			// neighbors
			std::cout << "neighbors = [";
			std::cout << &(*(fit->neighbor(0))) << ",";
			std::cout << &(*(fit->neighbor(1))) << ",";
			std::cout << &(*(fit->neighbor(2)));
			std::cout << "]" << std::endl;
		}
	}
	std::cout.flush();
}


bool read_incoming_datas(const Polygon_with_holes& map)
{
    CDT cdt;
    Pwh_list_2 difference;

	/*for (Polygon::Vertex_iterator vit=map.outer_boundary().vertices_begin();
								vit!=map.outer_boundary().vertices_end();++vit)
	{
		log(*vit);
	}*/
	
    if (!trouer_polygon(map, difference)) {
		log_err("impossible de trouer le polygon");
		return false;
	}

    if (difference.size() > 0)
    {

		for (Pwh_list_2::const_iterator pwhit=difference.begin(); pwhit != difference.end(); ++pwhit)
		{
			if (! pwhit->is_unbounded() )
			{
				insert_polygon_with_holes(cdt, *pwhit);
			}
			else
				log_err("poly is bounded");
		}
	}
	else
	{
		insert_polygon_with_holes(cdt, map);
	}
    
    //Mark facets that are inside the domain bounded by the polygon
    mark_domains(cdt);

    std::cout << count_facets(cdt) << std::endl;
    
    print_facets(cdt);

    return true;
}


int main()
{
	Polygon_with_holes map;
	
	ask_map(map);

	while(read_incoming_datas(map));
	
    return 0;
}

