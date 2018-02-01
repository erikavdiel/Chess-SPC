/*
 * SPArrayList.c
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */
#ifndef SPARRAYLIST_H_
#define SPARRAYLIST_H_

#define NULL 0
#include <stdbool.h>
#include <stdlib.h>

#include "SPArrayList.h"
#include "SPChessMove.h"

/**
 * SPArrayList summary:
 *
 * A container that represents a fixed size list. The capcity of the list
 * is specified at the creation. The container supports typical list
 * functionalities with the addition of random access as in arrays.
 * Upon insertion, if the maximum capacity is reached then an error message is
 * returned and the list is not affected.
 */



typedef struct sp_array_list_t {
	CHMove* elements;
	int actualSize;
	int maxSize;
} SPArrayList;



/**
 * A type used for errors
 */
typedef enum sp_array_list_message_t {
	SP_ARRAY_LIST_SUCCESS,
	SP_ARRAY_LIST_INVALID_ARGUMENT,
	SP_ARRAY_LIST_FULL,
	SP_ARRAY_LIST_EMPTY
} SP_ARRAY_LIST_MESSAGE;



/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int maxSize){
	SPArrayList* pArray = (SPArrayList*) malloc (sizeof(SPArrayList));
	CHMove *elements = (CHMove*) calloc(maxSize+1, sizeof(CHMove));
	if (elements == 0 || maxSize<=0){
		return NULL;
	}
	pArray->actualSize = 0;
	pArray->maxSize = maxSize;
	pArray->elements = elements;
	return pArray;
}



/**
 * Checks if an instant of array list - src is NULL.
 *	@param src - the source array list.
 *	@return
 *	True if src is NULL
 *	NULL otherwise
 */
bool assertSrc(SPArrayList* src){
	if(src==NULL){
		return true;
	}
	return false;
}



/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list.
 */
void spArrayListDestroy(SPArrayList* src){
	if (assertSrc(src)){
		return;
	}
	free(src->elements);
	free(src);
}



/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, CHMove* elem, int index){
	if (assertSrc(src) || index>src->actualSize+1){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	for (int i = src->actualSize-1; i >= index; i--  ){
		src->elements[i+1]=src->elements[i];
	}
	src->elements[index] = *elem;
	src->actualSize +=1;
	return SP_ARRAY_LIST_SUCCESS;
}



/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, CHMove *elem){
	 return spArrayListAddAt( src, elem, 0);
 }



/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, CHMove *elem){
	return spArrayListAddAt( src, elem, src->actualSize );
}



/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src){
	if((assertSrc(src)) || src->actualSize == 0){
		return true;
	}
	return false;
}



/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @param index - The index from where the new element will be removed.
 *                The index is 0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (index>(src->actualSize-1) || assertSrc(src)){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize==0){
		return SP_ARRAY_LIST_EMPTY;
	}
	for (int i = index; i<src->actualSize; i++){
		src->elements[i] = src->elements[i+1];
 	}
	src->actualSize -=1;
	src->elements[src->actualSize].emptyMove = true;
	return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise.
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if(spArrayListIsEmpty(src)){
			return SP_ARRAY_LIST_EMPTY;
		}
	return spArrayListRemoveAt(src, src->actualSize - 1);
}

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
CHMove* spArrayListGetAt(SPArrayList* src, int index){
	if(spArrayListIsEmpty(src)){
		return NULL;
	}
	return &(src->elements[index]);
}



/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
CHMove* spArrayListGetFirst(SPArrayList* src){
	if(spArrayListIsEmpty(src)){
		return 0;
	}
	return spArrayListGetAt(src, 0);
}



/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
CHMove* spArrayListGetLast(SPArrayList* src){
	if(spArrayListIsEmpty(src)){
		return 0;
	}
	return spArrayListGetAt(src, src->actualSize -1);
}



/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}



/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int spArrayListSize(SPArrayList* src){
	return src->actualSize;
}



/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool spArrayListIsFull(SPArrayList* src){
	if (src->actualSize==src->maxSize){
		return true;
	}
	return false;
}


/**
 * Copies (if possible) the content of one list to the end of other list.
 * @param:
 * small - the list to be copied.
 * big - the list that will contain the content of 'small'.
 * @return:
 * False - if the max capacity of 'big' list is smaller then the length of 'small' list or
 * if there is not enough room for all the arguments.
 * True - if all the arguments of 'small' have been copied to the end of 'big'.
 */
bool SPArrayListCopyIntoList(SPArrayList* big, SPArrayList* small){
	if (big->maxSize < small->actualSize){
		return false;
	}
	for (int i=0; i<small->actualSize; i++){
		if (spArrayListIsFull(big)){
			return false;
		}
		spArrayListAddLast(big, spArrayListGetAt(small, i));
	}
	return true;
}


/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
SPArrayList* spArrayListCopy(SPArrayList* src){
	SPArrayList* array =(SPArrayList*) malloc (sizeof(SPArrayList));
	CHMove* elements = calloc(src->maxSize+1, sizeof(CHMove));
	array->maxSize = src->maxSize;
	array->actualSize = src->actualSize;
	//assert(elements == 0);
	if (elements == 0 || assertSrc(src)){
		return NULL;
	}
	for (int i=0; i<(src->actualSize); i++){
		*(elements+i) = *(spArrayListGetAt(src, i));
	}
	array->elements = elements;
	return array;
}


#endif


