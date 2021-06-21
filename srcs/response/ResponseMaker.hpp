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
# include "../parser/RequestParser.hpp"

class ResponseMaker{
	private:
		std::string get(RequestParser &parser);
		std::string post(RequestParser &parser);
	public:
		ResponseMaker();
		~ResponseMaker();
		std::string makeResponse(const char *request);
};

#endif
