/*
 * Graph1.h
 *
 *  Created on: Mar 27, 2012
 *      Author: owner
 */






#ifndef GRAPH1_H_
#define GRAPH1_H_




#define INF 10000000.0
#define NODE_ITER list<nodetype>::iterator
#define EDGE_ITER list<edgetype>::iterator



struct edgetype{
	double cp;
	int from_node; //from node
	int to_node; //to node
};

struct nodetype{
	int info;
	list<edgetype> arcptr;

	nodetype(int n)
	{info=n;
	}
};




class Graph
{
	list<nodetype> g;
	vector<double> exs;
	vector<int> ht;

public:
	NODE_ITER get_node(int n);
	EDGE_ITER get_edge(int f,int t,bool &found);
	void add_node(int i);
	void add_nodes(int n);
	void add_edge(int f,int t,double cp=0,int fl=0);
	void update_edge(int f,int t,double cp);
	EDGE_ITER remove_edge(int f,int t);
	void scut(int s);
	void display_edges();


	vector<EDGE_ITER> bfs_path(int s,int t);
	void augment(vector<EDGE_ITER> v);
	void edmond_karp(int s,int t);


	void init_preflow(int s,queue<int> &q);
	void relabel(int u);
	void discharge(int u,int s,int t,queue<int> &q);
	void push_relabel(int s,int t);
};


NODE_ITER Graph::get_node(int n)
{
	NODE_ITER  gi;
	for(gi=g.begin();gi!=g.end();gi++)
	{
		if((*gi).info==n) return gi;
	}

	return g.end();
}

EDGE_ITER Graph::get_edge(int f,int t,bool &found)
{
	found=true;
	NODE_ITER  nit=get_node(f);

	EDGE_ITER eit;
	for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
	{
		if((*eit).to_node==t) return eit;
	}

	found=false;
	return (*nit).arcptr.end();
}

void Graph::add_node(int i)
{
	nodetype t(i);
	g.push_back(t);
}

void Graph::add_nodes(int n)
{
	for(int i=0;i<n;i++)
	{
		nodetype t(i);
		g.push_back(t);
	}
}

void Graph::add_edge(int f,int t,double cp,int fl)
{
	NODE_ITER  gi=get_node(f);

	edgetype e;
	e.cp=cp;
	e.from_node=f;
	e.to_node=t;

	(*gi).arcptr.push_back(e);
}

void Graph::update_edge(int f,int t,double n)
{
	bool found;
	EDGE_ITER eit=get_edge(f,t,found);
	if(found==false) add_edge(f,t,n);
	else (*eit).cp+=n;
}



EDGE_ITER Graph::remove_edge(int f,int t)
{
	NODE_ITER  nit=get_node(f);
	bool dummy;
	EDGE_ITER eit=get_edge(f,t,dummy);

	return (*nit).arcptr.erase(eit);
	//return false;
}


void Graph::scut(int s)
{
	queue<int > q1;
	q1.push(s);
	map<int,int> visited;

	visited[s]=1;
	while(!q1.empty())
	{
		queue<int > q2;
		while(!q1.empty())
		{
			int curr=q1.front();
			q1.pop();

			NODE_ITER nit=get_node(curr);
			EDGE_ITER eit;

			for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
			{
				if(visited.count((*eit).to_node)==0)
				{q2.push((*eit).to_node);
				visited[(*eit).to_node]=1;
				printf("%d\n",(*eit).to_node);
				}
			}
		}

		while(!q2.empty()) {q1.push(q2.front());q2.pop();}

	}
}


void Graph::display_edges()
{
	NODE_ITER nit;
	EDGE_ITER eit;

	/*
	for(nit=g.begin(); nit!=g.end(); nit++)
	{
		for( eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
			cout<<(*eit).from_node<<" "<<(*eit).to_node<<" "<<(*eit).cp<<"\n";
	}

	cout<<endl;
	 */
	double flow=0;
	nit=g.end();nit--;
	for( eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
		flow+=(*eit).cp;


	cout<<"Max flow : "<<flow<<endl;

	cout<<"\nMinimum Cut(s-side) : \n";
	scut(0);
}









vector<EDGE_ITER> Graph::bfs_path(int s,int t)
{

	queue<int > q1;
	q1.push(s);
	map<int,int> par;
	bool nfound=true;


	par[s]=-1;
	while(!q1.empty()&&nfound)
	{
		queue<int > q2;
		while(!q1.empty()&& nfound)
		{
			int curr=q1.front();q1.pop();

			NODE_ITER nit=get_node(curr);
			EDGE_ITER eit;

			for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
			{
				if(par.count((*eit).to_node)==0)
				{q2.push((*eit).to_node);
				par[(*eit).to_node]=curr;
				}
				if((*eit).to_node==t) {nfound=false;break;}
			}
		}

		while(!q2.empty()) {q1.push(q2.front());q2.pop();}

	}



	vector<EDGE_ITER> v;
	if(nfound) return v;

	int curr=t,to;

	while(curr!=s)
	{
		to=curr;
		curr=par[curr];
		bool dummy;
		v.push_back(get_edge(curr,to,dummy));
	}

	reverse(v.begin(),v.end());
	return v;
}


void Graph::augment(vector<EDGE_ITER> v)
{
	double bneck=INF;

	for(int i=0;i<v.size();i++)
	{
		if((*v[i]).cp<bneck) bneck=(*v[i]).cp;
	}

	for(int i=0;i<v.size();i++)
	{
		(*v[i]).cp-=bneck;
		update_edge((*v[i]).to_node,(*v[i]).from_node,bneck);
		if((*v[i]).cp==0) remove_edge((*v[i]).from_node,(*v[i]).to_node);
	}


}


void Graph::edmond_karp(int s,int t)
{
	vector<EDGE_ITER> v=bfs_path(s,t);
	while(!v.empty())
	{
		augment(v);
		v=bfs_path(s,t);
	}
}



void Graph::init_preflow(int s,queue<int> &q)
{
	int n=g.size();
	exs.resize(n,0);
	ht.resize(n,0);
	ht[s]=n;

	NODE_ITER  nit=get_node(s);

	EDGE_ITER eit;
	for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); /*eit++*/)
	{
		exs[(*eit).to_node]=(*eit).cp;
		q.push((*eit).to_node);
		update_edge((*eit).to_node,(*eit).from_node,(*eit).cp);
		eit=remove_edge((*eit).from_node,(*eit).to_node);
	}


}

void Graph::relabel(int u)
{
	NODE_ITER  nit=get_node(u);
	EDGE_ITER eit;
	int minht=INF;

	for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end(); eit++)
	{
		if(ht[(*eit).to_node]<minht) minht=ht[(*eit).to_node];
	}

	ht[u]=minht+1;
}


void Graph::discharge(int u,int s,int t,queue<int> &q)
{
	NODE_ITER  nit=get_node(u);

	EDGE_ITER eit;
	for(eit=(*nit).arcptr.begin(); eit!=(*nit).arcptr.end() && exs[u]>0 ; eit++)
	{
		int to=(*eit).to_node;
		if(ht[to]==ht[u]-1) {
			double bneck= exs[u]<(*eit).cp? exs[u]:(*eit).cp;
			exs[u]-=bneck;
			(*eit).cp-=bneck;

			if(exs[to]==0 && to!=s && to!=t) q.push(to);
			if(to!=s) exs[to]+=bneck;

			update_edge((*eit).to_node,(*eit).from_node,bneck);

			if((*eit).cp==0) {eit=remove_edge((*eit).from_node,(*eit).to_node);eit--;}

		}
	}

	if(exs[u]>0) {relabel(u);q.push(u);}
}


void Graph::push_relabel(int s,int t)
{
	queue<int> q;
	init_preflow(s,q);

	while(!q.empty())
	{
		int u=q.front();q.pop();
		discharge(u,s,t,q);
	}
}



#endif /* GRAPH1_H_ */
