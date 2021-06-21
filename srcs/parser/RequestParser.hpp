/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:43:26 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/19 16:43:27 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP
# include <iostream>

class RequestParser
{
	private:
		std::string method;
	public:
		RequestParser(const char *request);
		~RequestParser();
		std::string getMethod(void) const;
};

#endif