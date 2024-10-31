idf_component_register(SRCS "main.c" 
                            "../../src/led.cpp" 
                            "../../src/ledc.cpp" 
                            "../../src/led_ctrl.cpp"
                            "led_driver.c"
                    INCLUDE_DIRS "." 
                                "../../inc")
