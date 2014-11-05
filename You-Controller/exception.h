//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_EXCEPTION_H_
#define YOU_CONTROLLER_EXCEPTION_H_

#include "exceptions/exception.h"
#include "exceptions/context_required_exception.h"
#include "exceptions/context_index_out_of_range_exception.h"
#include "You-NLP/exception.h"
#include "You-QueryEngine/exception.h"
#include "You-DataStore/exception.h"

namespace You {
namespace Controller {

typedef You::Utils::Exception Exception;

typedef You::QueryEngine::Exception::EmptyTaskDescriptionException
	EmptyTaskDescriptionException;

typedef You::QueryEngine::Exception::TaskNotFoundException
	TaskNotFoundException;

typedef You::QueryEngine::Exception::CircularDependencyException
	CircularDependencyException;

typedef You::QueryEngine::Exception::NotUndoAbleException
	NotUndoAbleException;

typedef You::NLP::ParserException ParserException;

typedef You::NLP::ParserTypeException ParserTypeException;

typedef You::NLP::ParseErrorException ParseErrorException;

typedef You::DataStore::XmlParseErrorException XmlParseErrorException;

typedef You::DataStore::XmlIOException XmlIOException;

typedef You::DataStore::NotWellFormedXmlException NotWellFormedXmlException;

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_EXCEPTION_H_
