cmake_minimum_required (VERSION 3.8)

project ("Uptime-Kuma-Cpp")

add_library(UptimeKumaCpp 
	
	"./sources/uptime.cpp"
	"./sources/provider/provider.cpp"
	"./sources/provider/worker_provider.cpp"
	"./sources/net/client.cpp"
)

if(CMAKE_KUMA_USE_HTTPLIB)
	target_sources(UptimeKumaCpp PRIVATE "./sources/net/httplib.cpp")

	target_link_libraries(UptimeKumaCpp PUBLIC httplib)
	target_compile_definitions(UptimeKumaCpp PUBLIC KUMA_USE_HTTPLIB=1)
endif()

if(CMAKE_KUMA_USE_BOOST)
	target_sources(UptimeKumaCpp PRIVATE "./sources/net/boost.cpp")

	find_package(Boost REQUIRED)
	find_package(OpenSSL REQUIRED)

	target_link_directories(UptimeKumaCpp PUBLIC ${Boost_LIBRARY_DIR_RELEASE})
	target_include_directories(UptimeKumaCpp PUBLIC ${OPENSSL_INCLUDE_DIR} PUBLIC ${Boost_INCLUDE_DIR})
	target_link_libraries(UptimeKumaCpp PUBLIC ${OPENSSL_LIBRARIES} PUBLIC ${Boost_LIBRARIES})

	target_compile_definitions(UptimeKumaCpp PUBLIC KUMA_USE_BOOST=1)
endif()

target_include_directories(UptimeKumaCpp 
	PUBLIC "./include"
)

target_compile_features(UptimeKumaCpp PRIVATE cxx_std_17)