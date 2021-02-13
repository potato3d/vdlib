/**
*	Tree iterator for visiting nodes in pre-order: 
*		1. node
*		2. left subtree 
*		3. right subtree
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   5-Mar-2008
*/
#ifndef _VDLIB_PREORDERITERATOR_H_
#define _VDLIB_PREORDERITERATOR_H_

#include <vdlib/Common.h>
#include <vdlib/Node.h>

namespace vdlib {

// This implementation does not check for invalid operations
// Please make sure iterator is !done() before calling any other functions
class PreOrderIterator
{
public:
	typedef std::vector<Node*> TraversalStack;

	// Do nothing
	inline PreOrderIterator();

	// Begin with root
	inline PreOrderIterator( Node* root );

	// Reset stack and push root to be visited next
	inline void begin( Node* root );

	// Current node at top of stack
	inline Node* current() const;
	inline Node* operator->() const;

	// Continue traversal
	inline void next();

	// Skip current node's subtrees
	inline void skip();

	// Any more nodes to traverse?
	inline bool done() const;

	// Clear stack but preserve allocated memory to be reused later
	inline void finish();

	// Clear stack and free all memory
	inline void clear();

	// Used to continue traversal to node's subtrees
	inline void pushChildren( Node* node );

private:
	TraversalStack _stack;
};

inline PreOrderIterator::PreOrderIterator()
{
	// empty
}

inline PreOrderIterator::PreOrderIterator( Node* root )
{
	begin( root );
}

inline void PreOrderIterator::begin( Node* root )
{
	finish();
	_stack.push_back( root );
}

inline Node* PreOrderIterator::current() const
{
	return _stack.back();
}

inline Node* PreOrderIterator::operator->() const
{
	return _stack.back();
}

inline void PreOrderIterator::next()
{
	Node* current = _stack.back();
	_stack.pop_back();
	pushChildren( current );
}

inline void PreOrderIterator::skip()
{
	_stack.pop_back();
}

inline bool PreOrderIterator::done() const
{
	return _stack.empty();
}

inline void PreOrderIterator::finish()
{
	_stack.resize( 0 );
}

inline void PreOrderIterator::clear()
{
	vr::vectorFreeMemory( _stack );
}

inline void PreOrderIterator::pushChildren( Node* node )
{
	if( node->getRightChild() != NULL )
		_stack.push_back( node->getRightChild() );

	if( node->getLeftChild() != NULL )
		_stack.push_back( node->getLeftChild() );
}

} // namespace vdlib

#endif // _VDLIB_PREORDERITERATOR_H_
