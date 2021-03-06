% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/mesh.R
\name{create.mesh.2D}
\alias{create.mesh.2D}
\title{Create a 2D triangular mesh}
\usage{
create.mesh.2D(nodes, nodesattributes = NA, segments = NA, holes = NA, 
                     triangles = NA, order = 1, verbosity = 0)
}
\arguments{
\item{nodes}{A #nodes-by-2 matrix containing the x and y coordinates of the mesh nodes.}

\item{nodesattributes}{A matrix with #nodes rows containing nodes' attributes. 
These are passed unchanged to the output. If a node is added during the triangulation process or mesh refinement, its attributes are computed  
by linear interpolation using the attributes of neighboring nodes. This functionality is for instance used to compute the value 
of a Dirichlet boundary condition at boundary nodes added during the triangulation process.}

\item{segments}{A #segments-by-2 matrix. Each row contains the row's indices in \code{nodes} of the vertices where the segment starts from and ends to.
Segments are edges that are not splitted during the triangulation process. These are for instance used to define the boundaries
of the domain. If this is input is NULL, it generates a triangulation over the
convex hull of the points specified in \code{nodes}.}

\item{holes}{A #holes-by-2 matrix containing the x and y coordinates of a point internal to each hole of the mesh. These points are used to carve holes
in the triangulation, when the domain has holes.}

\item{triangles}{A #triangles-by-3 (when \code{order} = 1) or #triangles-by-6 (when \code{order} = 2) matrix.
This option is used when a triangulation is already available. It specifies the triangles giving the row's indices in \code{nodes} of the triangles' vertices and (when \code{nodes} = 2) also if the triangles' edges midpoints. The triangles' vertices and midpoints are ordered as described 
at \cr https://www.cs.cmu.edu/~quake/triangle.highorder.html.
In this case the function \code{create.mesh.2D} is used to produce a complete mesh.2D object.}

\item{order}{Either '1' or '2'. It specifies wether each mesh triangle should be represented by 3 nodes (the triangle' vertices) or by 6 nodes (the triangle's vertices and midpoints). 
These are
respectively used for linear (order = 1) and quadratic (order = 2) Finite Elements. Default is \code{order} = 1.}

\item{verbosity}{This can be '0', '1' or '2'. It indicates the level of verbosity in the triangulation process. When \code{verbosity} = 0 no message is returned
during the triangulation. When \code{verbosity} = 2 the triangulation process is described step by step by displayed messages.
Default is \code{verbosity} = 0.}
}
\value{
An object of the class mesh.2D with the following output:
\item{\code{nodes}}{A #nodes-by-2 matrix containing the x and y coordinates of the mesh nodes.}
\item{\code{nodesmarkers}}{A vector of length #nodes, with entries either '1' or '0'. An entry '1' indicates that the corresponding node is a boundary node; an entry '0' indicates that the corresponding node is not a boundary node.}
\item{\code{nodesattributes}}{nodesattributes A matrix with #nodes rows containing nodes' attributes. 
These are passed unchanged to the output. If a node is added during the triangulation process or mesh refinement, its attributes are computed  
by linear interpolation using the attributes of neighboring nodes. This functionality is for instance used to compute the value 
of a Dirichlet boundary condition at boundary nodes added during the triangulation process.}
\item{\code{triangles}}{A #triangles-by-3 (when \code{order} = 1) or #triangles-by-6 (when \code{order} = 2) matrix.
This option is used when a triangulation is already available. It specifies the triangles giving the indices in \code{nodes} of the triangles' vertices and (when \code{nodes} = 2) also if the triangles' edges midpoints. The triangles' vertices and midpoints are ordered as described 
at  \cr https://www.cs.cmu.edu/~quake/triangle.highorder.html.}
\item{\code{segmentsmarker}}{A vector of length #segments with entries either '1' or '0'. An entry '1' indicates that the corresponding element in \code{segments} is a boundary segment;  
an entry '0' indicates that the corresponding segment is not a boundary segment.}
\item{\code{edges}}{A #edges-by-2 matrix containing all the edges of the triangles in the output triangulation. Each row contains the row's indices in \code{nodes}, indicating the nodes where the edge starts from and ends to.}
\item{\code{edgesmarkers}}{A vector of lenght #edges with entries either '1' or '0'. An entry '1' indicates that the corresponding element in \code{edge} is a boundary edge;  
an entry '0' indicates that the corresponding edge is not a boundary edge.}
\item{\code{neighbors}}{A #triangles-by-3 matrix. Each row contains the indices of the three neighbouring triangles. An entry '-1' indicates that 
one edge of the triangle is a boundary edge.}
\item{\code{holes}}{A #holes-by-2 matrix containing the x and y coordinates of a point internal to each hole of the mesh. These points are used to carve holes
in the triangulation, when the domain has holes.}
\item{\code{order}}{Either '1' or '2'. It specifies wether each mesh triangle should be represented by 3 nodes (the triangle' vertices) or by 6 nodes (the triangle's vertices and midpoints). 
These are respectively used for linear (order = 1) and quadratic (order = 2) Finite Elements. Default is \code{order} = 1.}
}
\description{
This function is a wrapper of the Triangle library (http://www.cs.cmu.edu/~quake/triangle.html). It can be used
to create a triangulation of the domain of interest starting from a list of points, to be used as triangles' vertices, and a list of segments, that define the domain boundary. The resulting
mesh is a Constrained Delaunay triangulation. This is constructed in a way to preserve segments provided in the input \code{segments} without splitting them. This imput can be used to define the boundaries
of the domain. If this imput is NULL, it generates a triangulation over the
convex hull of the points.
}
\examples{
## Upload the Meuse data
data(MeuseData)
## Create a triangulation on the convex hull of these data,
## where each data location is a triangle vertex
mesh <- create.mesh.2D(nodes = MeuseData[,c(2,3)], order = 1)
## Plot the mesh
plot(mesh)
## Upload a domain boundary for these data
data(MeuseBorder)
## Create a constrained Delaunay triangulation with the provided boundary 
## where each datalocation is a triangle vertex
mesh <- create.mesh.2D(nodes = MeuseData[,c(2,3)], segments = MeuseBorder, order = 1)
## Plot the mesh
plot(mesh)
}
\seealso{
\code{\link{refine.mesh.2D}}, \code{\link{create.FEM.basis}}
}
