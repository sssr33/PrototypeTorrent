#pragma once

enum class BencodeType : int{
	Unknown = -1,
	Integer,
	String,
	List,
	Dictionary
};