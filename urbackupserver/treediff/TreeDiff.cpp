/*************************************************************************
*    UrBackup - Client/Server backup system
*    Copyright (C) 2011-2014 Martin Raiber
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "TreeDiff.h"
#include "TreeReader.h"
#include <algorithm>

std::vector<size_t> TreeDiff::diffTrees(const std::string &t1, const std::string &t2, bool &error,
	std::vector<size_t> *deleted_ids, std::vector<size_t>* large_unchanged_subtrees)
{
	std::vector<size_t> ret;

	TreeReader r1;
	if(!r1.readTree(t1))
	{
		error=true;
		return ret;
	}

	TreeReader r2;
	if(!r2.readTree(t2))
	{
		error=true;
		return ret;
	}

	gatherDiffs(&(*r1.getNodes())[0], &(*r2.getNodes())[0], ret);
	if(deleted_ids!=NULL)
	{
		gatherDeletes(&(*r1.getNodes())[0], *deleted_ids);
		std::sort(deleted_ids->begin(), deleted_ids->end());
	}
	if(large_unchanged_subtrees!=NULL)
	{
		gatherLargeUnchangedSubtrees(&(*r2.getNodes())[0], *large_unchanged_subtrees);
		std::sort(large_unchanged_subtrees->begin(), large_unchanged_subtrees->end());
	}

	std::sort(ret.begin(), ret.end());

	return ret;
}

void TreeDiff::gatherDiffs(TreeNode *t1, TreeNode *t2, std::vector<size_t> &diffs)
{
	size_t nc_2=t2->getNumChildren();
	size_t nc_1=t1->getNumChildren();
	TreeNode *c2=t2->getFirstChild();
	bool did_subtree_change=false;
	while(c2!=NULL)
	{		
		bool found=false;
		TreeNode *c1=t1->getFirstChild();
		while(c1!=NULL)
		{
			if(c1->equals(*c2))
			{
				gatherDiffs(c1, c2, diffs);
				c2->setMappedNode(c1);
				c1->setMappedNode(c2);
				found=true;
				break;
			}
			c1=c1->getNextSibling();
		}

		if(!found)
		{
			diffs.push_back(c2->getId());
			if(!did_subtree_change)
			{
				subtreeChanged(c2);
				did_subtree_change=true;
			}
		}

		c2=c2->getNextSibling();
	}
}

void TreeDiff::gatherDeletes(TreeNode *t1, std::vector<size_t> &deleted_ids)
{
	TreeNode *c1=t1->getFirstChild();
	while(c1!=NULL)
	{
		if(c1->getMappedNode()==NULL)
		{
			deleted_ids.push_back(c1->getId());
		}
		gatherDeletes(c1, deleted_ids);
		c1=c1->getNextSibling();
	}
}

void TreeDiff::subtreeChanged(TreeNode* t2)
{
	TreeNode* p = t2->getParent();
	if(p==NULL) return;

	do
	{
		if(p->getSubtreeChanged())
		{
			return;
		}

		p->setSubtreeChanged(true);
		p = p->getParent();
	}
	while(p!=NULL);
}

void TreeDiff::gatherLargeUnchangedSubtrees( TreeNode *t2, std::vector<size_t> &large_unchanged_subtrees )
{
	TreeNode *c2=t2->getFirstChild();
	while(c2!=NULL)
	{
		if(!c2->getSubtreeChanged()
			&& c2->getMappedNode()!=NULL
			&& getTreesize(c2,10)>10)
		{
			large_unchanged_subtrees.push_back(c2->getId());
		}
		else
		{
			gatherLargeUnchangedSubtrees(c2, large_unchanged_subtrees);
		}
		c2=c2->getNextSibling();
	}
}

size_t TreeDiff::getTreesize( TreeNode* t, size_t limit )
{
	size_t treesize=1;
	TreeNode *c=t->getFirstChild();
	while(c!=NULL)
	{
		treesize+=getTreesize(c, limit);
		if(treesize>limit)
		{
			return treesize;
		}
		c=c->getNextSibling();
	}
	return treesize;
}
