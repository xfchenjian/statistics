/*
 * statis_tree.h
 *
 * Created on: 2013-3-8
 * Author: RES-JIANCHEN
 */

#ifndef STATIS_TREE_H_
#define STATIS_TREE_H_

#include <map>
#include <set>
#include <list>
#include <string>

using namespace std;

typedef std::map<int, int> StatisArry;

class StatisTree
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	struct _tagTreeNode
	{
		int _iNodeType;
		string _strValue;

		_tagTreeNode* _pPreNode;			//父节点
		list<_tagTreeNode*> _subNodes; //子节点列表

		StatisArry _statisIterms;	//统计项，<key,value>

		void operator = (const _tagTreeNode& other)
		{
			_iNodeType = other._iNodeType;
			_strValue = other._strValue;
			_pPreNode = other._pPreNode;

			_subNodes.clear();
			_subNodes.assign(other._subNodes.begin(), other._subNodes.end());

			_statisIterms.clear();
			_statisIterms.insert(other._statisIterms.begin(), other._statisIterms.end());
		}

		bool operator ==(const _tagTreeNode& other)
		{
			return ((_iNodeType == other._iNodeType) && (_strValue == other._strValue));
		}

		bool operator ==(const _tagTreeNode& other) const
		{
			return ((_iNodeType == other._iNodeType) && (_strValue == other._strValue));
		}

		_tagTreeNode* next();

	};

	typedef _tagTreeNode TreeNode;

	class FindNodeFunc
	{
	public:
		FindNodeFunc(int iType, const string strValue)
			:_iType(iType), _strValue(strValue)
		{

		}

		bool operator()(TreeNode* node)
		{
			return ((_iType == node->_iNodeType) && (_strValue == node->_strValue));
		}
	private:
		int _iType;
		string _strValue;
	};


public:

	typedef TreeNode value_type;

	///////////////////////////////////////////////////////////////////////////////////////////
	class Iterator
	{
	public:
		Iterator(TreeNode* ptr=NULL):_ptr(ptr){};

		TreeNode& operator* () {return *_ptr;};
		TreeNode* operator-> (){return _ptr;};

		Iterator& operator++ ()
		{
			_ptr = _ptr->next(); return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator== (const Iterator& other)const
		{
			return _ptr == other._ptr;
		}

		bool operator!= (const Iterator& other)const
		{
			return _ptr != other._ptr;
		}

	private:
		TreeNode* _ptr;

	};


    ///////////////////////////////////////////////////////////////////////////////////////////
	StatisTree();
	explicit StatisTree(const string& strValue);

	int init(const string& strValue);

	int insert(const int iNodeType, const string strValue, const int iStatisType, const int iStatisValue );
	Iterator root(){return _root;};

	Iterator end(){return _end;}

private:
	StatisTree(const StatisTree& other);

	string typeKey(const TreeNode* node);

private:
	Iterator _root;
	Iterator _end;

	map<string, map<int, set<string> > > _arry;	//记录每个叶子节点的所有上层节点

};

typedef StatisTree::TreeNode TreeNode;

#endif /* STATIS_TREE_H_ */
