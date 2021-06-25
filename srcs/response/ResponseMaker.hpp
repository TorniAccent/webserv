/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMaker.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 17:12:55 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/19 17:12:58 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEMAKER_HPP
# define RESPONSEMAKER_HPP
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <fcntl.h>
# include "../parser/RequestParser.hpp"
# include "../parser/CParser.hpp"

class ResponseMaker{
	private:
		RequestParser	RParser;
		ConfigParser	CParser;

	public:
		ResponseMaker();
		explicit ResponseMaker(ConfigParser &configParser);
		~ResponseMaker();

		std::string makeResponse(RequestParser &requestParser);
		std::string get(RequestParser &requestParser);
		std::string post(RequestParser &requestParser);
};

#endif
