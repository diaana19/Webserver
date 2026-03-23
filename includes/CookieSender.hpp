/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CookieSender.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 13:39:25 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 13:39:26 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIESENDER_HPP
#define COOKIESENDER_HPP

#include <string>

class CookieSender
{
    private: 

    public:
        CookieSender();
         ~CookieSender();
         std::string buildSetCookie(const std::string& name, const std::string& value, int maxAge, const std::string& path);
};
#endif

/* cookie es un dato q el servidor envia al browser y este lo guarda y lo devuelve
en cada request siguiente
1. Browser → Server:  GET /login
=====   PERSONA B  ====
2. Server  → Browser: HTTP/1.1 200 OK
                       Set-Cookie: session_id=abc123; Max-Age=3600; Path=/; HttpOnly
=> nombre=valor, la cookie en si | max-age=3600, expira en 3600 seg | path=/, valida para todas las rutas
=> HttpOnly, JS no puede leerla(seguridad) | secure, solo HTTPS
3. Browser guarda la cookie

4. Browser → Server:  GET /profile
                       Cookie: session_id=abc123        ← la devuelve automáticamente
5. Server valida session_id y sabe quién es
Persona A: lee el header COOKIE: q viene del browser y lo convierte en un mapa clave=valor
Persona B: construye el header SET-COOKIE: q va en la rpta del servidor hacia el browser "guarda esta cookie"
Persona C: session manager gestiona la memoria del servidor, crea ids unicos, los asocia a datos de sesion, los valida
===== FLUJO =====
=> request llega: persona A parsea "cookie: session_id=abc123"
persona c: valida si abc123 existe en _sessions
=> request sale: persona c crea nueva sesion -> devuelve "xyz789"
persona b: construye "Set-Cookie: session_id=xyz789", Max-Age=3600, Path=/"
se agrega al HttpResponse
*/