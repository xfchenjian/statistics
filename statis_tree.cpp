/*
 * statis_tree.cpp
 *
 * Created on: 2013-3-8
 * Author: RES-JIANCHEN
 */

#include "statis_tree.h"

#include <log/xcps_logger.h>

#include "def.h"

TreeNode* StatisTree::TreeNode::next()
{
	TreeNode* curNode = this;

	//有子节点返回子节点
	if(!_subNodes.empty())
	{
		return (*_subNodes.begin());
	}

	while(curNode->_iNodeType != TREE_NODE_TYPE_ROOT)
	{
		//无子节点的返回下一个兄弟节点
		list<TreeNode*>& bros = curNode->_pPreNode->_subNodes;
		list<TreeNode*>::iterator curIndex = find(bros.begin(), bros.end(), curNode);
		if(++curIndex != bros.end())
		{
			return *curIndex;
		}

		curNode = curNode->_pPreNode;
	}

	return NULL;
}


StatisTree::StatisTree():_root(NULL),_end(NULL)
{

}

StatisTree::StatisTree(const StatisTree& other):_root(NULL),_end(NULL)
{

}

StatisTree::StatisTree(const string& strValue):_root(NULL),_end(NULL)
{
	init(strValue);
}

/**
 * 统计结构的初始化，root节点的生成，root节点的_iNodeType为TREE_NODE_TYPE_ROOT，
 * _iNodeValue统计的业务ID
 * @param iRootValue 统计业务的ID
 * @return 0成功，其他失败
 */
int StatisTree::init(const string& strValue)
{
	if(_root != _end)
	{
		LOG->error() << LOG_PREFIX << "the statis tree is already init !" <<endl;
		return -1;
	}

	_root = new TreeNode();
	_root->_iNodeType = TREE_NODE_TYPE_ROOT;
	_root->_strValue = strValue;
	_root->_pPreNode = NULL;
	_root->_statisIterms.clear();
	_root->_subNodes.clear();

	return 0;
}


int StatisTree::insert(const int iNodeType, const string strValue, const int iStatisType, const int iStatisValue )
{
	if(_end == _root)
	{
		init("root");
	}

	LOG->debug() << LOG_PREFIX << "Node: " << strValue << "\tstatis type: " << iStatisType << "\tstatis value: " << iStatisValue <<endl;

	string strLastNode;
	if(TREE_NODE_TYPE_MAN == iNodeType)
	{
		strLastNode = strValue.substr(strValue.rfind('-') + 1);
	}

	TreeNode* pNode = &(*_root);
	pNode->_statisIterms[iStatisType] += iStatisValue;

	int iIndex = 1;
	string strItem = getItem(strValue, iIndex);
	while(!strItem.empty())
	{
		list<TreeNode*>::iterator iter = find_if(pNode->_subNodes.begin(), pNode->_subNodes.end(), FindNodeFunc(pNode->_iNodeType + 1, strItem));
		if(iter == pNode->_subNodes.end())
		{
			TreeNode* ptmp = new TreeNode;
			ptmp->_iNodeType = pNode->_iNodeType + 1;
			ptmp->_strValue = strItem;
			ptmp->_pPreNode = pNode;
			ptmp->_statisIterms[iStatisType] = iStatisValue;

			if(TREE_NODE_TYPE_MAN == iNodeType)
			{
				string strNodeKey = typeKey(ptmp);
				_arry[strLastNode][iStatisType].insert(strNodeKey);
			}

			pNode->_subNodes.push_back(ptmp);
			pNode = ptmp;
		}
		else
		{
			if(TREE_NODE_TYPE_MAN == iNodeType)
			{
				string strNodeKey = typeKey((*iter));
				set<string>::iterator ptr = _arry[strLastNode][iStatisType].find(strNodeKey);
				if(ptr == _arry[strLastNode][iStatisType].end())
				{
					(*iter)->_statisIterms[iStatisType] += iStatisValue;
					_arry[strLastNode][iStatisType].insert(strNodeKey);
				}
			}
			else
				(*iter)->_statisIterms[iStatisType] += iStatisValue;

			pNode = (*iter);
		}

		LOG->debug() << LOG_PREFIX << "node type:" << pNode->_iNodeType <<"\tnode:" << pNode->_strValue << "\t" << iStatisType
					<< ":\t" << pNode->_statisIterms[iStatisType] <<endl;

		//next
		strItem.clear();
		strItem = getItem(strValue, ++iIndex);
	}

	return 0;
}

string StatisTree::typeKey(const TreeNode* node)
{
	switch(node->_iNodeType)
	{
	case TREE_NODE_TYPE_PROVINCE:
		return node->_strValue;
	case TREE_NODE_TYPE_CITY:
		return (node->_pPreNode->_strValue + node->_strValue);
	case TREE_NODE_TYPE_AREA:
		return (node->_pPreNode->_pPreNode->_strValue + node->_pPreNode->_strValue + node->_strValue);
	case TREE_NODE_TYPE_SCHOOL:
		return node->_strValue;
	case TREE_NODE_TYPE_GRADE:
		return (node->_pPreNode->_strValue + node->_strValue);
	case TREE_NODE_TYPE_CLASS:
		return node->_strValue;
	default:
		return string("");
	}
}
