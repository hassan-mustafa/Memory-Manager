#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<int> split(string str) {
	vector<int> arguments;
	int size = str.length();
	string temp;
	for (int i = 0; i <= size; i++) {
		if (i == str.length()) {
			arguments.push_back(stoi(temp));
			break;
		}
		if (str[i] == ' ') {
			arguments.push_back(stoi(temp));
			temp = "";
		}
		else {
			temp += str[i];
		}
	}
	return arguments;
}
struct segment {
	int id; int size;
};
struct segment_entry {
	int id, base, limit;
	string name;
};
struct partition {
	int start, end;
};
class process {
	int id, num_of_segs, segment_at, segment_table_at;
	segment *segments_arr;
	segment_entry *segment_table_arr;
public:
	process(int NUMBER_OF_SEGMENTS) :segment_at(0), num_of_segs(NUMBER_OF_SEGMENTS) {
		segments_arr = new segment[NUMBER_OF_SEGMENTS];
	}
	
	int get_num_segments() {
		return num_of_segs;
	}

	void set_id(int a) {
		id = a;
	}
	int get_id() {
		return id;
	}
	void add_segment(segment a) {
		if (segment_at < num_of_segs) {
			segments_arr[segment_at++] = a;
		}
	}
	
	segment *get_segments_arr() {
		return segments_arr;
	}
	void create_seg_table()
	{
		segment_table_arr = new segment_entry[num_of_segs];
	}
	void erase_seg_table()
	{
		delete[] segment_table_arr;
	}
	void add_segment_entry(segment_entry a,int z) {
		if (segment_table_at < num_of_segs) {
			segment_table_arr[z] = a;
		}
	}
	segment_entry *get_segment_table() {
		return segment_table_arr;
	}
	void print(){
		segment *seg = get_segments_arr();

		for (int z = 0; z < num_of_segs; z++)
		{

			cout << seg[z].id << endl << seg[z].size << endl;
		}
	}
	void print_segentry(){
		segment_entry *seg = get_segment_table();
		for (int z = 0; z < num_of_segs; z++)
		{
			cout << "table seg id:\t" << seg[z].id << endl << "base:\t" << seg[z].base << endl << "limit:\t" << seg[z].limit << endl;
		}
	}

};
void sortBystart(vector<partition>&);
void combine_holes(vector<partition> &,int);
void add_holes(vector<partition> & holes_list, vector <int> arguments,int memsize)
{
	for (int i = 0; i < arguments[0]; i++)
	{
		partition new_par;
		new_par.start = arguments[i * 2 + 1];
		new_par.end = (new_par.start) + arguments[2 + i * 2];
		if (new_par.end>memsize)
		{
			holes_list.clear();
			cout << "please enter holes within memory size" << endl;
			return;
		}
		holes_list.push_back(new_par);
	}
	combine_holes(holes_list, memsize);
}
void add_P(vector <process> &p_list, vector <int> argumentsP)
{
	process newp = process(argumentsP[0]);
	for (int j = 0; j < argumentsP[0]; j++)
	{
		segment newseg;
		newseg.id = argumentsP[(j * 2) + 1];
		newseg.size = argumentsP[2 + j * 2];
		newp.add_segment(newseg);
	}
	p_list.push_back(newp);
}
void sortBystart(vector<partition> &f) {
	partition temp;
	for (int i = 0; i < f.size(); i++) {
		for (int j = 0; j < f.size(); j++) {
			if (f[i].start < f[j].start) {
				temp = f[i];
				f[i] = f[j];
				f[j] = temp;
			}
		}
	}
}
void combine_holes(vector<partition> & holes_list,int memsize) {
	sortBystart(holes_list);
	partition new_par;
	new_par.start = memsize+1;
	new_par.end = 0;
	holes_list.push_back(new_par);
	int count = 0;
	int i = 0, j = 1;
	partition temp = holes_list[i];
	while (i < j && j < holes_list.size()) {
		if (temp.end == holes_list[j].start || temp.end > holes_list[j].start) {
			temp.end = holes_list[j++].end;
		}
		else {
			holes_list[count++] = temp;
			i = j;
			j++;
			temp = holes_list[i];
		}
	}
	holes_list.resize(count);
}
void addfilledpartioins(vector<partition> holes_list, vector<partition> &old_list, int memsize)
{
	for (int i = 0; i <= holes_list.size(); i++)
	{
		if (i == 0)
		{
			partition new_old;
			new_old.start = 0;
			new_old.end = holes_list[i].start;
			old_list.push_back(new_old);
		}
		else if (i == holes_list.size())
		{
			partition new_oldlast;
			new_oldlast.start = holes_list[i - 1].end;
			new_oldlast.end = memsize;
			old_list.push_back(new_oldlast);
			if (new_oldlast.start == new_oldlast.end)
				old_list.pop_back();
		}
		else if (i != 0 && i<holes_list.size())
		{
			partition new_olds;
			new_olds.start = holes_list[i - 1].end;
			new_olds.end = holes_list[i].start;
			old_list.push_back(new_olds);
		}
	}
	
}
void deallocatefilledparts(vector<partition> &holes_list, vector<partition> &old_list,int i,int memsize)
{
	partition newps;
	newps.start = old_list[i].start;
	newps.end = old_list[i].end;
	old_list.erase(old_list.begin() + i);
	holes_list.push_back(newps);
	combine_holes(holes_list,memsize);
}
bool checkvalidity(process s, vector<partition> holes_list)
{
	int count = 0;
	segment *seg = s.get_segments_arr();
	for (int j = 0; j < s.get_num_segments(); j++)
	{
		for (int t = 0; t < holes_list.size(); t++)
		{
			if ((seg[j].size) <= (holes_list[t].end - holes_list[t].start))
			{
				holes_list[t].start += seg[j].size;
				count++;
				break;
			}
		}
	}
	if (s.get_num_segments() == count)
		return 1;
	else return 0;
}
void allocateFirstFit(vector<partition> & holes_list, process &p_list)
{
		if (checkvalidity(p_list, holes_list))
		{
			p_list.create_seg_table();
			segment *segoo = p_list.get_segments_arr();
			for (int z = 0; z < p_list.get_num_segments(); z++)
			{
				for (int q = 0; q < holes_list.size(); q++)
				{
					if ((segoo[z].size) <= (holes_list[q].end - holes_list[q].start))
					{
						segment_entry new_segment_entry;
						new_segment_entry.id = z;
						new_segment_entry.base = holes_list[q].start;
						new_segment_entry.limit = (segoo[z].size);
						p_list.add_segment_entry(new_segment_entry,z);
						holes_list[q].start += segoo[z].size;
						if (holes_list[q].end == holes_list[q].start)
						{
							holes_list.erase(holes_list.begin() + q);
						}
						break;
					}

				}

			}
			p_list.print_segentry();
		}
		else { cout << "segments doesn't fit" << endl; return; }
}
void deallocate(process &s, vector<partition> & holes_list,int memsize){
	segment_entry *seg = s.get_segment_table();
	for (int i = 0; i < s.get_num_segments(); i++)
	{
		partition x;
		x.start = seg[i].base;
		x.end = seg[i].base + seg[i].limit;
		holes_list.push_back(x);
	}
	combine_holes(holes_list,memsize);
	s.erase_seg_table();
}
void sortbysize(vector<partition> &f)
{
	partition temp;
	for (int i = 0; i < f.size(); i++) {
		for (int j = 0; j < f.size(); j++) {
			if ((f[i].end - f[i].start) < (f[j].end - f[j].start)) {
				temp = f[i];
				f[i] = f[j];
				f[j] = temp;
			}
		}
	}
}
bool checkvaliditybe(process s, vector<partition> holes_list)
{
	int count = 0;
	segment *seg = s.get_segments_arr();
	for (int j = 0; j < s.get_num_segments(); j++)
	{
		sortbysize(holes_list);
		for (int t = 0; t < holes_list.size(); t++)
		{
			if ((seg[j].size) <= (holes_list[t].end - holes_list[t].start))
			{
				holes_list[t].start += seg[j].size;
				count++;
				break;
			}
		}
	}
	
	if (s.get_num_segments() == count)
		return 1;
	else return 0;
}
void allocateBestfit(vector<partition> & holes_list, process &p_list)
{
		if (checkvaliditybe(p_list, holes_list))
		{
			p_list.create_seg_table();
			segment *segoo = p_list.get_segments_arr();
			for (int z = 0; z < p_list.get_num_segments(); z++)
			{
				sortbysize(holes_list);
				for (int q = 0; q < holes_list.size(); q++)
				{
					if ((segoo[z].size) <= (holes_list[q].end - holes_list[q].start))
					{
						segment_entry new_segment_entry;
						new_segment_entry.id = z;
						new_segment_entry.base = holes_list[q].start;
						new_segment_entry.limit = (segoo[z].size);
						p_list.add_segment_entry(new_segment_entry,z);
						holes_list[q].start += segoo[z].size;
						if (holes_list[q].end == holes_list[q].start)
						{
							holes_list.erase(holes_list.begin() + q);
						}
						break;
					}

				}

			}
			sortBystart(holes_list);
			p_list.print_segentry();
		}
		else { cout << "segments doesn't fit" << endl; return; }
}
int main()
{
	cout << "please enter your memorysize" << endl;
	string g;
	getline(cin, g);
	int memsize = (stoi(g));
	string h;
	cout << "please enter number of holes, hole starting address and holes size respectefully" << endl;
	getline(cin, h);
	string p;
	cout << "please enter number of segments, each seg id, and segment size respectfully" << endl;
	getline(cin, p);
	cout << "choose 1 for first fit or 0 for best fit" << endl;
	int x;
	cin >> x;
	vector <int> arguments = split(h);
	vector<int> argumentsP = split(p);
	vector <process> ps_list;
	vector <partition> h_list;
	vector <partition> old_process;
	add_P(ps_list, argumentsP);
	add_holes(h_list, arguments,memsize);
	addfilledpartioins(h_list, old_process, stoi(g));
	cout << "holes and already filled partitions:" << endl << "holes:" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
		cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
		cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	cout << "already filled partitions:" << endl;
	for (int j = 0; j < old_process.size(); j++)
	{
		cout << "part \t" << j << "\t start address:\t" << old_process[j].start << endl;
		cout << "part \t" << j << "\t end address:\t" << old_process[j].end << endl;
	}
	deallocatefilledparts(h_list, old_process, 0, memsize);
	deallocatefilledparts(h_list, old_process, 0, memsize);
	deallocatefilledparts(h_list, old_process, 0, memsize);
	cout << "holes and already filled partitions after deallcoating the first filled partition:" << endl << "holes" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
		cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
		cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	cout << "already filled partitions:" << endl;
	for (int j = 0; j < old_process.size(); j++)
	{
		cout << "part \t" << j << "\t start address:\t" << old_process[j].start << endl;
		cout << "part \t" << j << "\t end address:\t" << old_process[j].end << endl;
	}
	if (x == 1)
	allocateFirstFit(h_list, ps_list[0]);
	else if (x == 0)
	allocateBestfit(h_list, ps_list[0]);
	cout << "after allocation:" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
	cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
	cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	deallocate(ps_list[0], h_list,memsize);
	cout << "After deallocation" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
	cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
	cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	//segment_entry sec;
	//sec.id = 0;
	//sec.base = 1;
	//sec.limit = 1;
	//ps_list[0].add_segment_entry(sec, 0);
	cout << "segments table after deallocation:" << endl;
	ps_list[0].print_segentry();
	cout << "segments table after reallocation:" << endl;
	allocateFirstFit(h_list, ps_list[0]);
	cout << "holes after reallocation:" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
		cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
		cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	/*cout << "holes and already filled partitions:"<<endl<<"holes:" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
	cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
	cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	cout << "already filled partitions:" << endl;
	for (int j = 0; j < old_process.size(); j++)
	{
	cout << "part \t" << j << "\t start address:\t" << old_process[j].start << endl;
	cout << "part \t" << j << "\t end address:\t" << old_process[j].end << endl;
	}
	cout << "holes and already filled partitions after deallcoating the first filled partition:"<<endl<<"holes" << endl;
	for (int i = 0; i < h_list.size(); i++)
	{
	cout << "hole \t" << i << "\t start address:\t" << h_list[i].start << endl;
	cout << "hole \t" << i << "\t end address:\t" << h_list[i].end << endl;
	}
	cout << "already filled partitions:" << endl;
	for (int j = 0; j < old_process.size(); j++)
	{
	cout << "part \t" << j << "\t start address:\t" << old_process[j].start << endl;
	cout << "part \t" << j << "\t end address:\t" << old_process[j].end << endl;
	}*/
}